<?php

class Simulacion
{
  private $id_simulacion;
  private $uid;
  private $nombre_simulacion;
  private $datos_medios;
  private $listaPartidas;

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct($id_simulacion, $uid)
  {
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $id_simulacion, '=');
    $resultado = $query->execute();

    while ($record = $resultado->fetchAssoc()) {
      $this->id_simulacion = $id_simulacion;
      $this->uid = $uid;
      $this->nombre_simulacion = $record['nombre_simulacion'];
      $this->datos_medios['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->datos_medios['revoluciones_medias'] = $this->retrieveAverageData('rpm');
      if ($uid > 0) {
        $this->datos_medios['velocidad_media_usuario'] = $this->retrieveAverageData('velocidad', $uid);
        $this->datos_medios['revoluciones_medias_usuario'] = $this->retrieveAverageData('rpm', $uid);
      }
    }
  }

  /**
   * @return mixed
   */
  public function getDatosMedios()
  {
    return $this->datos_medios;
  }

  /**
   * @param mixed $datos_medios
   */
  public function setDatosMedios($datos_medios)
  {
    $this->datos_medios = $datos_medios;
  }

  /**
   * @return mixed
   */
  public function getIdSimulacion()
  {
    return $this->id_simulacion;
  }

  /**
   * @param mixed $id_simulacion
   */
  public function setIdSimulacion($id_simulacion)
  {
    $this->id_simulacion = $id_simulacion;
  }

  /**
   * @return mixed
   */
  public function getNombreSimulacion()
  {
    return $this->nombre_simulacion;
  }

  /**
   * @param mixed $nombre_simulacion
   */
  public function setNombreSimulacion($nombre_simulacion)
  {
    $this->nombre_simulacion = $nombre_simulacion;
  }

  /**
   * @return mixed
   */
  public function getUid()
  {
    return $this->uid;
  }

  /**
   * @param mixed $uid
   */
  public function setUid($uid)
  {
    $this->uid = $uid;
  }

  /**
   * @return ListaPartidas
   */
  public function getListaPartidas()
  {
    if(!isset($this->listaPartidas)) {
      $this->loadListaPartidas();
    }

    return $this->listaPartidas;
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
    if (isset($uid)) {
      $query->condition('p.uid', $uid, '=');
    }
    $records = $query->execute();

    $resultado = null;
    while ($record = $records->fetchAssoc()) {
      $resultado = $record['suma_total'] / $count;
    }

    return $resultado;
  }

  /**
   * Carga la Lista de Partidas de la Simulación.
   * Usamos la Lazy Initialization
   */
  private function loadListaPartidas() {
    $this->listaPartidas = new ListaPartidas();

    $query = db_select('rjsim_partida', 'p');
    $query->fields('p', array('id_partida'))
      ->condition('uid', $this->uid, '=')
      ->condition('id_simulacion', $this->id_simulacion, '=');

    $resultado = $query->execute();

    while ($record = $resultado->fetchAssoc()) {
      $this->listaPartidas->add(Partida::loadPartidaById($record['id_partida']));
    }
  }
}