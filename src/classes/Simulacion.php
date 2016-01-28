<?php

class Simulacion
{
  private $id_simulacion;
  private $nombre_simulacion;
  private $uid;
  private $datos_medios;
  private $listaPartidas;

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct($id_simulacion, $uid)
  {
    if (!is_numeric($id_simulacion) || !is_numeric($uid)) {
      throw new InvalidArgumentException("El id de la simulación y el UID tienen que ser un entero.");
    }

    $provider = FactoryDataProvider::createDataProvider();

    $this->id_simulacion = $id_simulacion;
    $this->nombre_simulacion = $provider->getNombreSimulacionFromID($id_simulacion);
    $this->uid = $uid;
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
      $provider = FactoryDataProvider::createDataProvider();
      $this->listaPartidas = $provider->loadListaPartidasBySimulation($this);
    }

    return $this->listaPartidas;
  }

  /* ********************************************************************************* */
  /*                                      METHODS                                      */
  /* ********************************************************************************* */
  /**
   * @return string URL para ver los datos de una partida.
   */
  public function getURLToSimulacionPage($type = null)
  {
    $url = base_path() . 'simulaciones/' . $this->getIdSimulacion() . '/partidas';

    if (isset($type) && $type == 'html_link') {
      return '<a href="' .$url. '">Ver partidas de simulación</a>';
    }

    return $url;
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
}