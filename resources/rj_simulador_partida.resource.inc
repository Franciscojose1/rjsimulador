<?php
function partida_resource_definition()
{
  $resources = array(
    'rj_simulador_partida' => array(
      'operations' => array(
        'create' => array(
          'help' => 'Crea una nueva partida junto con sus datos e infracciones',
          'file' => array('type' => 'inc', 'module' => 'rj_simulador', 'name' => 'resources/rj_simulador_partida.resource'),
          'callback' => '_rj_simulador_partida_add',
          'access callback' => 'user_access',
          'access arguments' => array('crear partidas'),
          'access arguments append' => FALSE,
          'args' => array(
            array(
              'name' => 'partida',
              'type' => 'struct',
              'description' => 'El objeto partida',
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
        'retrieve' => array(
          'help' => 'Recupera una partida',
          'file' => array('type' => 'inc', 'module' => 'rj_simulador', 'name' => 'resources/rj_simulador_partida.resource'),
          'callback' => '_rj_simulador_partida_retrieve',
          'access callback' => 'user_access',
          'access arguments' => array('crear partidas'),
          'access arguments append' => FALSE,
          'args' => array(
            array(
              'name' => 'id_partida',
              'type' => 'int',
              'description' => 'El id de la partida a recuperar',
              'source' => array('path' => 0),
              'optional' => FALSE,
            ),
          ),
        ),
      ),
    ),
  );
  return $resources;
}

function _rj_simulador_partida_add($partida)
{
  $usuario = user_uid_optional_load();

  // Create a transaction; if we catch exception, rollback and store watchdog exception
  $transaction = db_transaction();
  try {
    $newPartida = new Partida($usuario->uid, REQUEST_TIME, $partida['id_simulacion']);
    foreach ($partida['infracciones'] as $infraccion) {
      $record = new Infraccion($infraccion['instante'], $infraccion['id_infraccion']);
      $record->setPosicionX($infraccion['posicion_x']);
      $record->setPosicionY($infraccion['posicion_y']);
      $record->setPosicionZ($infraccion['posicion_z']);
      $record->setObservaciones($infraccion['observaciones']);
      $newPartida->addInfraccion($record);
    }
    foreach ($partida['datos'] as $dato) {
      $record = new DatoInstantaneo($dato['instante'], $dato['velocidad'], $dato['rpm'], $dato['marcha']);
      $record->setPosicionX($dato['posicion_x']);
      $record->setPosicionY($dato['posicion_y']);
      $record->setPosicionZ($dato['posicion_z']);
      $newPartida->addDato($record);
    }
    $newPartida->savePartida();
    return array("message" => "Partida creada correctamente");
  } catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('Insertando partida.', $e);
    return services_error('Error al insertar la partida en la BBDD.', 406, array("error" => "Error al guardar la partida: " . $e->getMessage()));
  }
}

function _rj_simulador_partida_retrieve($id_partida)
{
  try {
    $partida = Partida::loadPartidaById($id_partida, Partida::PARTIDA_INFRACCIONES_DATOS);
    return $partida->convertPropertiesToArrayForServices();
  } catch (Exception $e) {
    watchdog_exception('Recuperando partida.', $e);
    return services_error('Error recuperando partida.', 406, array("error" => 'Error al recuperar la partida: ' . $e->getMessage()));
  }
}

function _rj_simulador_retrieve_statistics($id_simulacion)
{
  try {
    $usuario = user_uid_optional_load();
    $simulacion = new Simulacion($id_simulacion, $usuario->uid);
    return $simulacion;
  } catch (Exception $e) {
    watchdog_exception('Recuperando estadísticas de simulación.', $e);
    return services_error('Error recuperando estadísticas.', 406, array("error" => 'Error al recuperar estadísticas de la simulación: ' . $e->getMessage()));
  }
}