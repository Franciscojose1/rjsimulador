<?php

class DatoInstantaneo implements ServicesAdapterInterface {
  /* ********************************************************************************* */
  /*                                   PROPERTIES                                      */
  /* ********************************************************************************* */
  /* @var int $id_partida */
  private $id_partida;
  /* @var float $instante */
  private $instante;
  /* @var float $posicion_x */
  private $posicion_x;
  /* @var float $posicion_y */
  private $posicion_y;
  /* @var float $posicion_z */
  private $posicion_z;
  /* @var float $velocidad */
  private $velocidad;
  /* @var float $rpm */
  private $rpm;
  /* @var int $marcha */
  private $marcha;
  /* @var float $consumo_instantaneo */
  private $consumo_instantaneo;
  /* @var float $consumo_total */
  private $consumo_total;

  /* ********************************************************************************* */
  /*                                   CONSTRUCTOR                                     */
  /* ********************************************************************************* */
  function __construct($instante, $velocidad, $rpm, $marcha) {
    $this->setInstante($instante);
    $this->setVelocidad($velocidad);
    $this->setRpm($rpm);
    $this->setMarcha($marcha);
  }

  /* ********************************************************************************* */
  /*                                    ACCESSORS                                      */
  /* ********************************************************************************* */
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
      throw new InvalidArgumentException("El ID de partida debe ser un entero.");
    }
  }

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
  public function setInstante($instante) {
    if (is_numeric($instante)) {
      $this->instante = floatval($instante);
    }
    else {
      throw new InvalidArgumentException("El instante del dato debe ser un número.");
    }
  }

  /**
   * @return int
   */
  public function getRpm() {
    return $this->rpm;
  }

  /**
   * @param float $rpm
   * @throws InvalidArgumentException
   */
  public function setRpm($rpm) {
    if (is_numeric($rpm)) {
      $this->rpm = floatval($rpm);
    }
    else {
      throw new InvalidArgumentException("Las RPM deben ser un número.");
    }
  }

  /**
   * @return int
   */
  public function getMarcha() {
    return $this->marcha;
  }

  /**
   * @param int $marcha
   * @throws InvalidArgumentException
   */
  public function setMarcha($marcha) {
    if (is_numeric($marcha)) {
      $this->marcha = intval($marcha);
    }
    else {
      throw new InvalidArgumentException("La marcha debe ser un entero.");
    }
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
   * @return array Position as array with keys [x], [y] y [z]
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
   * @return float
   */
  public function getVelocidad() {
    return $this->velocidad;
  }

  /**
   * @param float $velocidad
   * @throws InvalidArgumentException
   */
  public function setVelocidad($velocidad) {
    if (is_numeric($velocidad)) {
      $this->velocidad = floatval($velocidad);
    }
    else {
      throw new InvalidArgumentException("La velocidad debe ser un número.");
    }
  }

  /**
   * @return float
   */
  public function getConsumoInstantaneo() {
    return $this->consumo_instantaneo;
  }

  /**
   * @param float $consumo_instantaneo
   * @throws InvalidArgumentException
   */
  public function setConsumoInstantaneo($consumo_instantaneo) {
    if (is_numeric($consumo_instantaneo)) {
      $this->consumo_instantaneo = floatval($consumo_instantaneo);
    }
    else {
      throw new InvalidArgumentException("El Consumo Instantáneo tiene que ser un número decimal.");
    }
  }

  /**
   * @return float
   */
  public function getConsumoTotal() {
    return $this->consumo_total;
  }

  /**
   * @param float $consumo_total
   * @throws InvalidArgumentException
   */
  public function setConsumoTotal($consumo_total) {
    if (is_numeric($consumo_total)) {
      $this->consumo_total = floatval($consumo_total);
    }
    else {
      throw new InvalidArgumentException("El Consumo Total " . $consumo_total . " tiene que ser un número decimal.");
    }
  }


  /* ********************************************************************************* */
  /*                                     METHODS                                       */
  /* ********************************************************************************* */
  /**
   * @throws Exception
   */
  public function save() {
    if ($this->getIdPartida() == NULL || $this->getInstante() == NULL) {
      throw new Exception("Los campos ID de Partida e Instante son necesarios para almacenar un nuevo Dato.");
    }

    $saver = FactoryDataSaver::createDataSaver();
    $saver->saveDatoInstantaneo($this);
  }

  /**
   * @inheritdoc
   */
  public function convertPropertiesToArray() {
    return get_object_vars($this);
  }
}