<?php
function partida_resource_definition()
{
  $resources = array(
    'rj_simulador_partidas' => array(
      'operations' => array(
        'create' => array(
          'help' => 'Crea una nueva partida junto con sus datos e infracciones',
          'callback' => '_rj_simulador_add',
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
      ),
      'actions' => array(
        'retrieve_statistics' => array(
          'help' => 'Recupera las estadísticas de un tipo de simulación',
          'callback' => '_rj_simulador_retrieve_statistics',
          'access callback' => 'user_access',
          'access arguments' => array('gestionar estadisticas'),
          'access arguments append' => FALSE,
          'args' => array(
            array(
              'name' => 'id_simulacion',
              'type' => 'int',
              'description' => 'El ID de la simulación',
              'source' => array('data'=>'id_simulacion'),
              'optional' => FALSE,
            ),
          ),
        ),
      ),
    ),
  );
  return $resources;
}

function _rj_simulador_add($partida)
{
  $usuario = user_uid_optional_load();
  // Check if user is not Anonymous
  if ($usuario->uid < 1) {
    return services_error("Los usuarios anónimos no pueden crear partidas", 406);
  }
  $newPartida = new Partida();
  $newPartida->uid = $usuario->uid;
  $newPartida->fecha = REQUEST_TIME;
  $newPartida->id_simulacion = $partida['id_simulacion'];
  $newPartida->infracciones = $partida['infracciones'];
  $newPartida->datos = $partida['datos'];

  $transaction = db_transaction();
  try {
    $newPartida->insertInDatabase();

    return array("message" => "Partida creada correctamente");
  } catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('Insertando partida', $e);
    return services_error('Error al insertar la partida en la BBDD. ' . $e->getMessage(), 406);
  }
}

function _rj_simulador_retrieve_statistics($id_simulacion)
{
  try {
    $usuario = user_uid_optional_load();
    $simulacion = new Simulacion($id_simulacion,$usuario->uid);
    return $simulacion;
  } catch (Exception $e) {
    return services_error('Error recuperando estadísticas: ' . $e->getMessage(), 406);
  }
}