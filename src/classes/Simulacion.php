<?php

class Simulacion {
  /* @var int $id_simulacion El ID de la Simulación */
  private $id_simulacion;
  /* @var string $nombre_simulacion */
  private $nombre_simulacion;
  /* @var UsuarioSimulacion $usuario */
  private $usuario;
  /* @var ListaPartidas $listaPartidas */
  private $listaPartidas;

  /**
   * Simulacion constructor.
   * @param int $id_simulacion
   * @param \UsuarioSimulacion $usuario
   * @throws InvalidArgumentException Si los datos del constructor son erróneos.
   */
  public function __construct($id_simulacion, UsuarioSimulacion $usuario) {
    if (!is_numeric($id_simulacion)) {
      throw new InvalidArgumentException("El id de la simulación tiene que ser un entero.");
    }
    else if (!($usuario instanceof UsuarioSimulacion)) {
        throw new InvalidArgumentException("El usuario tiene que ser un objeto de tipo UsuarioSimulacion.");
    }

    $this->setIdSimulacion($id_simulacion);
    $this->setUsuario($usuario);
  }

  /**
   * @return int
   */
  public function getIdSimulacion() {
    return $this->id_simulacion;
  }

  /**
   * @param int $id_simulacion
   */
  public function setIdSimulacion($id_simulacion) {
    $this->id_simulacion = intval($id_simulacion);
  }

  /**
   * @return string
   * @throws \Exception Posible error recuperando el nombre de la simulación.
   */
  public function getNombreSimulacion() {
    if(!isset($this->nombre_simulacion)) {
      $provider = FactoryDataManager::createDataProvider();
      $this->setNombreSimulacion($provider->loadNombreSimulacionFromID($this->getIdSimulacion()));
    }

    return $this->nombre_simulacion;
  }

  /**
   * @param string $nombre_simulacion
   */
  public function setNombreSimulacion($nombre_simulacion) {
    $this->nombre_simulacion = $nombre_simulacion;
  }

  /**
   * @return UsuarioSimulacion
   */
  public function getUsuario() {
    return $this->usuario;
  }

  /**
   * @param UsuarioSimulacion $usuario
   */
  public function setUsuario(UsuarioSimulacion $usuario) {
    $this->usuario = $usuario;
  }

  /**
   * @return \ListaPartidas
   * @throws \Exception Error recuperando la lista de partidas.
   */
  public function getListaPartidas() {
    if (!isset($this->listaPartidas)) {
      $provider = FactoryDataManager::createDataProvider();
      $this->listaPartidas = $provider->loadListaPartidasBySimulation($this);
    }

    return $this->listaPartidas;
  }

  /* ********************************************************************************* */
  /*                                      METHODS                                      */
  /* ********************************************************************************* */
  /**
   * Devuelve la URL para acceder a una simualción.
   * @param bool $adminMode Si debe devolver urls de administrador o no.
   * @param string|null $type Si es "html_link" se devuelve la URL como un enlace HTML.
   * @return string URL para ver los datos de una simulación.
   */
  public function getURLToSimulacionPage($adminMode, $type = NULL) {
    $url = base_path();

    if ($adminMode) {
      $url .= 'admin/simulaciones_usuarios/' . $this->getUsuario()->getUid() . '/';
    }

    $url .= 'simulaciones/' . $this->getIdSimulacion() . '/partidas';

    if (isset($type) && $type == 'html_link') {
      return '<a href="' . $url . '">Ver partidas de simulación</a>';
    }

    return $url;
  }
}