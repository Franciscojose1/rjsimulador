<?php

class Simulacion
{
  public $id_simulacion;
  public $nombre_simulacion;
  public $datos_medios;

  /*
     Constructor Simulacion
   */
  public function __construct()
  {
    $argv = func_get_args();
    switch (func_num_args()) {
      case 0:
        self::__construct0();
        break;
      case 1:
        self::__construct1($argv[0]);
        break;
      case 2:
        self::__construct2($argv[0], $argv[1]);
        break;
    }
  }

  /*
    Constructor Empty
  */
  public function __construct0()
  {
  }

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct1($id_simulacion)
  {
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $id_simulacion, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $this->id_simulacion = $id_simulacion;
      $this->nombre_simulacion = $record['nombre_simulacion'];
      $this->datos_medios['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->datos_medios['revoluciones_medias'] = $this->retrieveAverageData('revoluciones');
    }
  }

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct2($id_simulacion, $uid)
  {
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $id_simulacion, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $this->id_simulacion = $id_simulacion;
      $this->nombre_simulacion = $record['nombre_simulacion'];
      $this->datos_medios['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->datos_medios['revoluciones_medias'] = $this->retrieveAverageData('revoluciones');
      if ($uid > 0) {
        $this->datos_medios['velocidad_media_usuario'] = $this->retrieveAverageData('velocidad', $uid);
        $this->datos_medios['revoluciones_medias_usuario'] = $this->retrieveAverageData('revoluciones', $uid);
      }
    }
  }

  public function retrieveAverageData($parameter, $uid = null)
  {
    $query = db_select('rjsim_partida', 'p');
    $query->innerJoin('rjsim_datos_partida', 'dp', 'p.id_partida = dp.id_partida');
    $query->fields('p', array('id_partida'))
      ->condition('p.id_simulacion', $this->id_simulacion, '=');
    if (isset($uid) && $uid != null) {
      $query->condition('p.uid', $uid, '=');
    }
    $count = $query->execute()->rowCount();
    if ($count == 0) {
      return null;
    }

    $query = db_select('rjsim_partida', 'p');
    $query->innerJoin('rjsim_datos_partida', 'dp', 'p.id_partida = dp.id_partida');
    $query->addExpression('SUM(' . $parameter . ')', 'suma_total');
    $query->condition('p.id_simulacion', $this->id_simulacion, '=');
    if (isset($uid) && $uid != null) {
      $query->condition('p.uid', $uid, '=');
    }
    $records = $query->execute();

    $resultado = null;
    while ($record = $records->fetchAssoc()) {
      $resultado = $record['suma_total'] / $count;
    }

    return $resultado;
  }
}