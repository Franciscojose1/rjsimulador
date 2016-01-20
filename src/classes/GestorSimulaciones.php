<?php

class GestorSimulaciones
{
  const SIMULACION_UNO = 1;
  const SIMULACION_DOS = 2;
  const SIMULACION_TRES = 3;
  const SIMULACION_CUATRO = 4;
  const SIMULACION_CINCO = 5;

  /* @var Simulacion[] */
  private $simulaciones;
  /* @var int|null */
  private $uid;

  /**
   * @param array $idsSimulacionesACargar
   * @param null|int $uid
   */
  public function __construct($idsSimulacionesACargar, $uid = null)
  {
    foreach ($idsSimulacionesACargar as $id) {
      dpm($id);
      $this->simulaciones[] = new Simulacion($id, $uid);
      dpm($this->simulaciones);
    }

    $this->uid = $uid;
  }

  /**
   * @return int|null
   */
  public function getUid()
  {
    return $this->uid;
  }

  /**
   * @param int|null $uid
   */
  public function setUid($uid)
  {
    $this->uid = $uid;
  }

  /**
   * @return Simulacion[]
   */
  public function getSimulaciones()
  {
    return $this->simulaciones;
  }

  /**
   * @param Simulacion[] $simulaciones
   */
  public function setSimulaciones($simulaciones)
  {
    $this->simulaciones = $simulaciones;
  }


  /**
   * @return Partida[] Listado de partidas de este usuario para esta simulación (o para todas)
   */
  function retrieveSimulationsPartidas()
  {
    $partidas = array();

    foreach ($this->simulaciones as $simulacion) {
      $partidas = array_merge($partidas, $simulacion->retrieveListOfPartidas());
    }
    error_log(print_r($this->simulaciones, TRUE));
    dpm($partidas);
    return $partidas;
  }
} 