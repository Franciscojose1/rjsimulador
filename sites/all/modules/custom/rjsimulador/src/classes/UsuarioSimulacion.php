<?php

class UsuarioSimulacion {
  /* @var stdClass $user La entidad usuario */
  private $user;
  /* @var ListaSimulaciones $listaSimulaciones */
  private $listaSimulaciones;

  public function __construct(stdClass $user) {
    $this->setUser($user);
  }

  /**
   * @return stdClass
   */
  private function getUser() {
    return $this->user;
  }

  /**
   * @param stdClass $user
   */
  private function setUser($user) {
    $this->user = $user;
  }

  /**
   * @return ListaSimulaciones
   */
  public function getListaSimulaciones() {
    if (!isset($this->listaSimulaciones)) {
      $provider = FactoryDataProvider::createDataProvider();
      $this->listaSimulaciones = $provider->loadListaSimulacionesByUsuario($this);
    }
    return $this->listaSimulaciones;
  }

  /**
   * @return int El UID del usuario
   */
  public function getUid() {
    return $this->getUser()->uid;
  }

  /**
   * @return string
   */
  public function getName() {
    return $this->getUser()->name;
  }

  /**
   * @return string
   */
  public function getMail() {
    return $this->getUser()->mail;
  }

  /**
   * @param bool $asDate Si queremos la fecha como Timestamp o como un objeto DateTime.
   * @return DateTime|int La fecha de creación como DateTime o como Timestamp.
   */
  public function getCreationDate($asDate = FALSE) {
    return $asDate ? (new DateTime())->setTimestamp($this->getUser()->created) : $this->getUser()->created;
  }

  /**
   * @param bool $asDate Si queremos la fecha como Timestamp o como un objeto DateTime.
   * @return DateTime|int La fecha de login como DateTime o como Timestamp.
   */
  public function getLoginDate($asDate = FALSE) {
    return $asDate ? (new DateTime())->setTimestamp($this->getUser()->login) : $this->getUser()->login;
  }

  /**
   * @param bool $asDate Si queremos la fecha como Timestamp o como un objeto DateTime.
   * @return DateTime|int La fecha de útlimo acceso como DateTime o como Timestamp.
   */
  public function getLastAccessDate($asDate = FALSE) {
    return $asDate ? (new DateTime())->setTimestamp($this->getUser()->access) : $this->getUser()->access;
  }

  public function getAge() {
    $birthdate = (new DateTime())->setTimestamp($this->getUser()->field_birthdate['und'][0]['value']);
    $now = new DateTime();
    $interval = $now->diff($birthdate);
    return $interval->y;
  }

  public function getDrivingExperience() {
    $dateDrivingLicense = (new DateTime())->setTimestamp($this->getUser()->field_driving_experience['und'][0]['value']);
    $now = new DateTime();
    $interval = $now->diff($dateDrivingLicense);
    return $interval->y;
  }

  public function getAverageAnnualMileage() {
    return $this->getUser()->field_average_annual_mileage['und'][0]['value'];
  }

  public function isUsualVideogamePlayer() {
    return $this->getUser()->field_usual_videogame_player['und'][0]['value'] == 1;
  }

  /**
   * Método que devuelve todas las partidas de un usuario.
   * @return ListaPartidas Lista de todas las partidas de ese usuario de todas las simulaciones.
   */
  public function retrieveAllPartidas() {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getListaSimulaciones() as $simulacion) {
      $listaPartidas->mergeList($simulacion->getListaPartidas());
    }

    return $listaPartidas;
  }

  /**
   * Método que devuelve todas las partidas del usuario para una simulación en concreto.
   * @param int $idSimulation El id de la simulación para la que recuperar las partidas de los usuarios.
   * @return ListaPartidas Lista de todas las partidas de esa simulación..
   */
  public function retrieveAllPartidasByIdSimulacion($idSimulation) {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getListaSimulaciones() as $simulacion) {
      if ($simulacion->getIdSimulacion() == $idSimulation) {
        $listaPartidas->mergeList($simulacion->getListaPartidas());
        break;
      }
    }

    return $listaPartidas;
  }

  /**
   * @return int Número de partidas de este usuario.
   */
  public function countPartidas() {
    $numeroPartidas = 0;

    foreach ($this->getListaSimulaciones() as $simulacion) {
      $numeroPartidas += $simulacion->getListaPartidas()->count();
    }

    return $numeroPartidas;
  }
}