<?php
module_load_include('inc', 'rjsimulador', 'rjsimulador.forms.admin');

use Drupal\rjsimulador\Controllers\GestorSimulaciones;
use Drupal\rjsimulador\Partida;
use Drupal\rjsimulador\Infraccion;
use Drupal\rjsimulador\DatoInstantaneo;
use Drupal\rjsimulador\Constants;

/* *************************************************************************** */
/* *                    PÁGINA DE ADMIN SIMULACIONES                           */
/* *************************************************************************** */
function rjsimulador_simulaciones_usuarios_admin_page() {
  // Gestor de simulaciones
  try {
    $gestorSimulaciones = new GestorSimulaciones();
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<h3>' . $le->getMessage() . '</h3>');
    return $renderArray;
  }


  // Creamos la tabla a sacar para mostrar las partidas
  // Headers array
  $headers = array(
    array('data' => t('User ID'), 'field' => 'Uid', 'sort' => 'ASC'),
    array('data' => t('User Name'), 'field' => 'Name'),
    array('data' => t('Age')),
    array('data' => t('Driving experience')),
    array('data' => t('Average annual mileage')),
    array('data' => t('Usual videogame player?')),
    array('data' => t('Email')),
    array('data' => t('Created'), 'field' => 'Created'),
    array('data' => t('Last login'), 'field' => 'Login'),
    array('data' => t('Last access'), 'field' => 'LastAccess'),
    array('data' => t('Links'))
  );

  $listaUsuarios = $gestorSimulaciones->getListaTodosUsuarios();

  // Getting the current sort and order parameters from the url
  $order = tablesort_get_order($headers);
  $sort = tablesort_get_sort($headers);

  if (isset($order) && isset($sort)) {
    $listaUsuarios->sortBy($order['sql'], strtoupper($sort));
  }

  $rows = array();
  foreach ($listaUsuarios as $usuario) {
    $rows[] = array(
      array('data' => $usuario->getUid()),
      array('data' => $usuario->getName()),
      array('data' => $usuario->getAge()),
      array('data' => $usuario->getDrivingExperience()),
      array('data' => $usuario->getAverageAnnualMileage()),
      array('data' => $usuario->isUsualVideogamePlayer(true) ? t("Yes") : t("No") ),
      array('data' => $usuario->getMail()),
      array(
        'data' => $usuario->getCreationDate(true)->format("d-m-Y H:i:s")
      ),
      array(
        'data' => $usuario->getLoginDate(true)->format("d-m-Y H:i:s")
      ),
      array(
        'data' => $usuario->getLastAccessDate(true)->format("d-m-Y H:i:s")
      ),
      array('data' => l(t('Show info about simulations'), 'admin/simulaciones_analysis/' . $usuario->getUid() . '/simulaciones'))
    );
  }

  $renderArrayTableSimulaciones = NULL;
  $limit = 20;
  $page = pager_default_initialize(count($rows), $limit, 0);
  $offset = $limit * $page;
  $renderArrayTableSimulaciones =
    array(
      array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => array_slice($rows, $offset, $limit),
        '#empty' => t('There are no users with simulations in the database.')
      ),
      array(
        '#theme' => 'pager',
        '#element' => 0,
      ),
    );

  $renderArrayFinal = array(
    '#theme' => 'lista_simulaciones_partidas',
    '#main_content' => $renderArrayTableSimulaciones,
  );

  return $renderArrayFinal;
}

function rjsimulador_load_from_file($form, &$form_state) {
  $users = entity_load('user');

  $options = array();
  foreach ($users as $user) {
    $options[$user->uid] = $user->name;
  }

  $form['load_user'] = array(
    '#type' => 'select',
    '#title' => t('Select game user'),
    '#options' => $options,
    '#default_value' => 1,
    '#required' => TRUE
  );

  $form['load_date'] = array(
    '#type' => 'date',
    '#title' => t('Select game date'),
    '#required' => TRUE
  );

  $form['load_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('JSON Game files to load'),
    '#prefix' => '<div id="container-files">',
    '#suffix' => '</div>',
    '#tree' => TRUE
  );

  $form_state['total_files'] = isset($form_state['total_files']) ? $form_state['total_files'] : 1;

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'add_one_file') {
      $form_state['total_files']++;
    }

    if ($form_state['clicked_button']['#name'] == 'remove_one_file') {
      $form_state['total_files']--;
    }
  }

  $form_state['total_files'] = $form_state['total_files'] == 0 ? 1 : $form_state['total_files'];

  for ($index = 0; $index < $form_state['total_files']; $index++) {
    $form['load_files'][$index] = array(
      '#type' => 'managed_file',
      '#description' => t('Add JSON file to save new game'),
      '#upload_validators' => array('file_validate_extensions' => array('txt, log')),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  $form['add_button'] = array(
    '#type' => 'button',
    '#value' => t('Add new file'),
    '#name' => 'add_one_file',
    '#ajax' => array(
      'callback' => 'ajax_file_rebuild',
      'wrapper' => 'container-files',
    ),
  );

  $form['delete_button'] = array(
    '#type' => 'button',
    '#value' => t('Delete file'),
    '#name' => 'remove_one_file',
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'ajax_file_rebuild',
      'wrapper' => 'container-files',
    ),
  );

  return $form;
}

function ajax_file_rebuild($form, &$form_state) {
  return $form['load_files'];
}

function rjsimulador_load_from_file_submit($form, &$form_state) {
  $userId = $form_state['values']['load_user'];
  $submittedDate = $form_state['values']['load_date'];
  $dateTime = DateTime::createFromFormat('Y-m-d', $submittedDate['year'].'-'.$submittedDate['month'].'-'.$submittedDate['day']);
  $operations = array();

  // Gestionamos cada fichero como una operacion
  foreach($form_state['values']['load_files'] as $idFile) {
    $operations[] = array('json_file_process', array($userId, $dateTime->getTimestamp(), $idFile));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'json_load_finished',
    'title' => t('Processing JSON Files'),
    'init_message' => t('JSON Files Batch is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('JSON Files Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'rjsimulador') . '/rjsimulador.user_analysis.inc',
  );

  // Lanzamos el proceso en batch
  batch_set($batch);
}

function json_file_process($userId, $dateUnix, $file_id, &$context) {
  // Cargamos el fichero, recuoperamos el contenido y los parseamos a JSON
  $file = file_load($file_id);
  $content = file_get_contents($file->uri);

  $context['message'] = t('Parsing the file');
  $partida = drupal_json_decode($content);

  $tiposSimulaciones = Constants::getTiposSimulacion();
  if (!array_key_exists($partida["id_simulacion"], $tiposSimulaciones)) {
    throw new InvalidArgumentException("No existe una simulación con el ID " . $partida["id_simulacion"] . ".");
  }

  $newPartida = new Partida($userId, $dateUnix, $partida['id_simulacion']);
  $newPartida->setConsumoMedio($partida['consumo_medio']);
  $newPartida->setConsumoTotal($partida['consumo_total']);
  $newPartida->setTiempoTotal($partida['tiempo_total']);

  $tiposInfracciones = Constants::getTiposInfracciones();
  foreach ($partida['infracciones'] as $infraccion) {
    if (!array_key_exists($infraccion["id_infraccion"], $tiposInfracciones)) {
      throw new InvalidArgumentException("No existe una infracción con el ID " . $infraccion["id_infraccion"] . ".");
    }

    $record = new Infraccion($infraccion['instante'], $infraccion['id_infraccion']);
    $record->setPosicionX($infraccion['posicion_x']);
    $record->setPosicionY($infraccion['posicion_y']);
    $record->setPosicionZ($infraccion['posicion_z']);
    $record->setObservaciones($infraccion['observaciones']);
    $newPartida->getListaInfracciones()->add($record);
  }

  foreach ($partida['datos'] as $dato) {
    $record = new DatoInstantaneo($dato['instante'], $dato['velocidad'], $dato['rpm'], $dato['marcha']);
    $record->setPosicionX($dato['posicion_x']);
    $record->setPosicionY($dato['posicion_y']);
    $record->setPosicionZ($dato['posicion_z']);
    $record->setConsumoInstantaneo($dato['consumo_instantaneo']);
    $record->setConsumoTotal($dato['consumo_total']);
    $newPartida->getListaDatos()->add($record);
  }

  // Creamos una transacción para almacenar la partida; si algo falla hacemos rollback
  $transaction = db_transaction();
  try {
    $context['message'] = t('Creating game from file');
    $newPartida->save();
    // Hacemos commit deseteando la variable.
    unset($transaction);
  } catch (Exception $e) {
    $transaction->rollback();
    throw $e;
  }

  if (!isset($context['results']['total_files'])) {
    $context['results']['total_files'] = 1;
  } else {
    $context['results']['total_files']++;
  }

  // Damos la operación sobre el fichero por finalizada y pasamos a la siguiente
  $context['finished'] = 1;
}

/**
 * Batch 'finished' callback
 */
function json_load_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('All @total_files files have been successfully processed', array('@total_files' => $results['total_files'])));
  } else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}