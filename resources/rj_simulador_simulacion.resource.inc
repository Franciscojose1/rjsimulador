<?php
function simulacion_resource_definition()
{
  $resources = array(
    'rj_simulador_simulacion' => array(
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
              'source' => array('data' => 'id_simulacion'),
              'optional' => FALSE,
            ),
          ),
        ),
      ),
    ),
  );
  return $resources;
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