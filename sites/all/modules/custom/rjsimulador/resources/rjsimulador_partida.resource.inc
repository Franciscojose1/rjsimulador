<?php
use RJSimulador\Factory\FactoryDataManager;
use RJSimulador\Partida, RJSimulador\Infraccion, RJSimulador\DatoInstantaneo, RJSimulador\Constants;

function partida_resource_definition() {
  $resources = array(
    'rjsimulador_partida' => array(
      'operations' => array(
        'create' => array(
          'help' => 'Crea una nueva partida junto con sus datos e infracciones',
          'file' => array(
            'type' => 'inc',
            'module' => 'rjsimulador',
            'name' => 'resources/rjsimulador_partida.resource'
          ),
          'callback' => '_rjsimulador_partida_add',
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
          'file' => array(
            'type' => 'inc',
            'module' => 'rjsimulador',
            'name' => 'resources/rjsimulador_partida.resource'
          ),
          'callback' => '_rjsimulador_partida_retrieve',
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

function _rjsimulador_partida_add($partida) {
  $provider = FactoryDataManager::createDataProvider();
  $usuario = $provider->loadSimulatorUser();

  try {
    $tiposSimulaciones = Constants::getTiposSimulacion();
    if (!array_key_exists($partida["id_simulacion"], $tiposSimulaciones)) {
      throw new InvalidArgumentException("No existe una simulación con el ID " . $partida["id_simulacion"] . ".");
    }

    $newPartida = new Partida($usuario->getUid(), REQUEST_TIME, $partida['id_simulacion']);
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
      $newPartida->save();
      // Hacemos commit deseteando la variable.
      unset($transaction);
    } catch (Exception $e) {
      $transaction->rollback();
      throw $e;
    }

    return array("message" => "Partida creada correctamente");
  } catch (Exception $e) {
    watchdog_exception('rjsimulador', $e, 'Error inserting new Partida.');
    return services_error('Error al insertar la partida en la BBDD.', 406, array("error" => "Error al guardar la partida: " . $e->getMessage()));
  }
}

function _rjsimulador_partida_retrieve($id_partida) {
  try {
    $partida = Partida::loadById($id_partida);
    return $partida->convertPropertiesToArray();
  } catch (Exception $e) {
    watchdog_exception('rjsimulador', $e, 'Error retrieving Partida.');
    return services_error('Error recuperando partida.', 406, array("error" => 'Error al recuperar la partida: ' . $e->getMessage()));
  }
}