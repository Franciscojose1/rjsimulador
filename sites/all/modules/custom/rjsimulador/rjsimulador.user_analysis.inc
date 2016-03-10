<?php
module_load_include('inc', 'rjsimulador', 'rjsimulador.forms.admin');

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
