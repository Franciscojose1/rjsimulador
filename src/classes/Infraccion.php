<?php

class Infraccion implements ServicesAdapterInterface {
  /* ********************************************************************************* */
  /*                                      PROPERTIES                                   */
  /* ********************************************************************************* */
  /* @var int $id_partida */
  private $id_partida;
  /* @var float $instante */
  private $instante;
  /* @var int $id_infraccion */
  private $id_infraccion;
  /* @var float $posicion_x */
  private $posicion_x;
  /* @var float $posicion_y */
  private $posicion_y;
  /* @var float $posicion_z */
  private $posicion_z;
  /* @var string $nombre_infraccion */
  private $nombre_infraccion;
  /* @var string $observaciones */
  private $observaciones;

  /* ********************************************************************************* */
  /*                                     CONSTRUCTOR                                   */
  /* ********************************************************************************* */
  /**
   * Infraccion constructor.
   * @param $instante
   * @param $id_infraccion
   */
  function __construct($instante, $id_infraccion) {
    $this->setInstante($instante);
    $this->setIdInfraccion($id_infraccion);
  }

  /* ********************************************************************************* */
  /*                                      ACCESSORS                                    */
  /* ********************************************************************************* */
  /**
   * @return float
   */
  public function getInstante() {
    return $this->instante;
  }

  /**
   * @param float $instante
   * @throws InvalidArgumentException
   */
  private function setInstante($instante) {
    if (is_numeric($instante)) {
      $this->instante = floatval($instante);
    }
    else {
      throw new InvalidArgumentException("El instante de la infracción debe ser un número.");
    }
  }

  /**
   * @return int
   */
  public function getIdInfraccion() {
    return $this->id_infraccion;
  }

  /**
   * @param int $id_infraccion
   * @throws InvalidArgumentException
   */
  private function setIdInfraccion($id_infraccion) {
    if (is_numeric($id_infraccion)) {
      $this->id_infraccion = intval($id_infraccion);
    }
    else {
      throw new InvalidArgumentException("El ID de la infracción debe ser un entero.");
    }
  }

  /**
   * @return int
   */
  public function getIdPartida() {
    return $this->id_partida;
  }

  /**
   * @param int $id_partida
   * @throws InvalidArgumentException
   */
  public function setIdPartida($id_partida) {
    if (is_numeric($id_partida)) {
      $this->id_partida = intval($id_partida);
    }
    else {
      throw new InvalidArgumentException("El ID de la partida debe ser un entero.");
    }
  }

  /**
   * @return string
   */
  public function getNombreInfraccion() {
    if (!isset($this->nombre_infraccion)) {
      $this->setNombreInfraccion(Constants::getNombreInfraccion($this->getIdInfraccion()));
    }

    return $this->nombre_infraccion;
  }

  /**
   * @param string $nombre_infraccion
   */
  private function setNombreInfraccion($nombre_infraccion) {
    $this->nombre_infraccion = $nombre_infraccion;
  }

  /**
   * @return float
   */
  public function getPosicionX() {
    return $this->posicion_x;
  }

  /**
   * @param float $posicion_x
   * @throws InvalidArgumentException
   */
  public function setPosicionX($posicion_x) {
    if (is_numeric($posicion_x)) {
      $this->posicion_x = floatval($posicion_x);
    }
    else {
      throw new InvalidArgumentException("La posición X debe ser un número.");
    }
  }

  /**
   * @return float
   */
  public function getPosicionY() {
    return $this->posicion_y;
  }

  /**
   * @param float $posicion_y
   * @throws InvalidArgumentException
   */
  public function setPosicionY($posicion_y) {
    if (is_numeric($posicion_y)) {
      $this->posicion_y = floatval($posicion_y);
    }
    else {
      throw new InvalidArgumentException("La posición Y debe ser un número.");
    }
  }

  /**
   * @return float
   */
  public function getPosicionZ() {
    return $this->posicion_z;
  }

  /**
   * @param float $posicion_z
   * @throws InvalidArgumentException
   */
  public function setPosicionZ($posicion_z) {
    if (is_numeric($posicion_z)) {
      $this->posicion_z = floatval($posicion_z);
    }
    else {
      throw new InvalidArgumentException("La posición Z debe ser un número.");
    }
  }

  /**
   * @return array $posicion Position as array keys [x], [y] y [z]
   */
  public function getPosicion() {
    $posicion = array(
      'x' => $this->getPosicionX(),
      'y' => $this->getPosicionY(),
      'z' => $this->getPosicionZ()
    );
    return $posicion;
  }

  /**
   * @param array $posicion Position as array keys [x], [y] y [z]
   * @throws InvalidArgumentException
   */
  public function setPosicion($posicion) {
    if (is_array($posicion) && isset($posicion['x']) && isset($posicion['y']) && isset($posicion['z'])) {
      $this->setPosicionX($posicion['x']);
      $this->setPosicionY($posicion['y']);
      $this->setPosicionZ($posicion['z']);
    }
    else {
      throw new InvalidArgumentException("La posicion debe ser un array asociativo con x, y y z.");
    }
  }

  /**
   * @return string
   */
  public function getObservaciones() {
    return $this->observaciones;
  }

  /**
   * @param string $observaciones
   */
  public function setObservaciones($observaciones) {
    $this->observaciones = $observaciones;
  }

  /* ********************************************************************************* */
  /*                                       METHODS                                     */
  /* ********************************************************************************* */
  /**
   * Almacena el objeto de forma persistente.
   * @throws Exception Si ocurre un error durante el almacenamiento.
   */
  public function save() {
    if ($this->getIdPartida() == NULL || $this->getIdInfraccion() == NULL || $this->getInstante() == NULL) {
      throw new Exception("Los campos ID de Partida, Instante e ID de Infracción son necesarios para almacenar una nueva Infraccion");
    }

    $saver = FactoryDataManager::createDataSaver();
    $saver->saveInfraccion($this);
  }

  /**
   * Elimina la Infraccion de forma persistente.
   * @throws \Exception Cuando ocurre un error durante el borrado.
   */
  public function remove() {
    if ($this->getIdPartida() == NULL || $this->getInstante() == NULL || $this->getIdInfraccion()) {
      throw new Exception("Una Infraccion debe tener un ID, un Instante y un ID de Partida asociada para poder ser borrada.");
    }

    $deleter = FactoryDataManager::createDataRemover();
    $deleter->removeInfraccion($this);
  }

  /**
   * @inheritdoc
   */
  public function convertPropertiesToArray() {
    return get_object_vars($this);
  }
}