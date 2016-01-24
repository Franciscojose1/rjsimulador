<?php

class GestorSimulaciones
{
  const SIMULACION_UNO = 1;
  const SIMULACION_DOS = 2;
  const SIMULACION_TRES = 3;
  const SIMULACION_CUATRO = 4;
  const SIMULACION_CINCO = 5;

  /* @var ListaSimulaciones */
  private $listaSimulaciones;
  /* @var int|null */
  private $uid;

  /**
   * @param array $idsSimulacionesACargar
   * @param null|int $uid
   */
  public function __construct($idsSimulacionesACargar, $uid = null)
  {
    $this->listaSimulaciones = new ListaSimulaciones();
    foreach ($idsSimulacionesACargar as $id) {
      $this->listaSimulaciones->add(new Simulacion($id, $uid));
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
   * @return ListaSimulaciones
   */
  public function getListaSimulaciones()
  {
    return $this->listaSimulaciones;
  }


  /**
   * @return ListaPartidas Listado de partidas de este usuario para esta simulación (o para todas)
   */
  function retrieveAllSimulationsPartidas()
  {
    $listaPartidas = new ListaPartidas();

    foreach ($this->listaSimulaciones as $simulacion) {
      $listaPartidas->mergeList($simulacion->getListaPartidas());
    }

    return $listaPartidas;
  }
} 