<?php

/**
 * Class Simulacion Representa una simulación del Simulador.
 */
class Simulacion {
  /* @var int $id_simulacion El ID de la Simulación */
  private $id_simulacion;
  /* @var string $nombre_simulacion */
  private $nombre_simulacion;
  /* @var int $userUid */
  private $userUid;
  /* @var ListaPartidas $listaPartidas */
  private $listaPartidas;

  /**
   * Simulacion constructor.
   * @param int $id_simulacion
   * @param \UsuarioSimulacion $usuario
   * @throws InvalidArgumentException Si los datos del constructor son erróneos.
   */
  public function __construct($id_simulacion, $userUid) {
    if (!is_numeric($id_simulacion)) {
      throw new InvalidArgumentException("El id de la simulación tiene que ser un entero.");
    }
    else if (!is_numeric($userUid)) {
        throw new InvalidArgumentException("El uid del usuario tiene que ser un entero.");
    }

    $this->setIdSimulacion($id_simulacion);
    $this->setUserUid($userUid);
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
   * @return string EL nombre de la simulación.
   * @throws \LogicException Error recuperando el nombre de la simulación.
   */
  public function getNombreSimulacion() {
    if(!isset($this->nombre_simulacion)) {
      $this->setNombreSimulacion(Constants::getNombreSimulacion($this->getIdSimulacion()));
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
   * @return int El UID del usuario de la simulación.
   */
  public function getUserUid() {
    return $this->userUid;
  }

  /**
   * @param int $uid El uid del usuario de la simulación.
   */
  public function setUserUid($userUid) {
    $this->userUid = intval($userUid);
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
    $url = '';

    if ($adminMode) {
      $url .= 'admin/simulaciones_analysis/' . $this->getUserUid() . '/';
    }

    $url .= 'simulaciones/' . $this->getIdSimulacion() . '/partidas';

    if (isset($type) && $type == 'html_link') {
      return l(t('Show Simulation\'s Partidas'), $url);
    }

    return $url;
  }
}