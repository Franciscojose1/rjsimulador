<?php

class DatoInstantaneo implements ServicesAdapterInterface
{
  /* ********************************************************************************* */
  /*                                   PROPERTIES                                      */
  /* ********************************************************************************* */
  private $id_partida;
  private $instante;
  private $posicion_x;
  private $posicion_y;
  private $posicion_z;
  private $velocidad;
  private $rpm;
  private $marcha;

  /* ********************************************************************************* */
  /*                                   CONSTRUCTOR                                     */
  /* ********************************************************************************* */
  function __construct($instante, $velocidad, $rpm, $marcha)
  {
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
  public function getIdPartida()
  {
    return $this->id_partida;
  }

  /**
   * @param int $id_partida
   * @throws InvalidArgumentException
   */
  public function setIdPartida($id_partida)
  {
    if (is_numeric($id_partida)) {
      $this->id_partida = $id_partida;
    } else {
      throw new InvalidArgumentException("El ID de partida debe ser un entero.");
    }
  }

  /**
   * @return int
   */
  public function getInstante()
  {
    return $this->instante;
  }

  /**
   * @param mixed $instante
   * @throws InvalidArgumentException
   */
  public function setInstante($instante)
  {
    if (is_numeric($instante)) {
      $this->instante = $instante;
    } else {
      throw new InvalidArgumentException("El instante del dato debe ser un número.");
    }
  }

  /**
   * @return mixed
   */
  public function getRpm()
  {
    return $this->rpm;
  }

  /**
   * @param mixed $rpm
   * @throws InvalidArgumentException
   */
  public function setRpm($rpm)
  {
    if (is_numeric($rpm)) {
      $this->rpm = $rpm;
    } else {
      throw new InvalidArgumentException("Las RPM deben ser un número.");
    }
  }

  /**
   * @return int
   */
  public function getMarcha()
  {
    return $this->marcha;
  }

  /**
   * @param int $marcha
   * @throws InvalidArgumentException
   */
  public function setMarcha($marcha)
  {
    if (is_numeric($marcha)) {
      $this->marcha = $marcha;
    } else {
      throw new InvalidArgumentException("La marcha debe ser un entero.");
    }
  }

  /**
   * @return float
   */
  public function getPosicionX()
  {
    return $this->posicion_x;
  }

  /**
   * @param float $posicion_x
   * @throws InvalidArgumentException
   */
  public function setPosicionX($posicion_x)
  {
    if (is_numeric($posicion_x)) {
      $this->posicion_x = $posicion_x;
    } else {
      throw new InvalidArgumentException("La posición X debe ser un número.");
    }
  }

  /**
   * @return float
   */
  public function getPosicionY()
  {
    return $this->posicion_y;
  }

  /**
   * @param float $posicion_y
   * @throws InvalidArgumentException
   */
  public function setPosicionY($posicion_y)
  {
    if (is_numeric($posicion_y)) {
      $this->posicion_y = $posicion_y;
    } else {
      throw new InvalidArgumentException("La posición Y debe ser un número.");
    }
  }

  /**
   * @return float
   */
  public function getPosicionZ()
  {
    return $this->posicion_z;
  }

  /**
   * @param float $posicion_z
   * @throws InvalidArgumentException
   */
  public function setPosicionZ($posicion_z)
  {
    if (is_numeric($posicion_z)) {
      $this->posicion_z = $posicion_z;
    } else {
      throw new InvalidArgumentException("La posición Z debe ser un número.");
    }
  }

  /**
   * @return array $posicion
   */
  public function getPosicion()
  {
    $posicion = array('x' => $this->getPosicionX(), 'y' => $this->getPosicionY(), 'z' => $this->getPosicionZ());
    return $posicion;
  }

  /**
   * @param array $posicion
   * @throws InvalidArgumentException
   */
  public function setPosicion($posicion)
  {
    if (is_array($posicion) && isset($posicion['x']) && isset($posicion['y']) && isset($posicion['z'])) {
      $this->setPosicionX($posicion['x']);
      $this->setPosicionY($posicion['y']);
      $this->setPosicionZ($posicion['z']);
    } else {
      throw new InvalidArgumentException("La posicion debe ser un array asociativo con x, y y z.");
    }
  }

  /**
   * @return float
   */
  public function getVelocidad()
  {
    return $this->velocidad;
  }

  /**
   * @param float $velocidad
   * @throws InvalidArgumentException
   */
  public function setVelocidad($velocidad)
  {
    if (is_numeric($velocidad)) {
      $this->velocidad = $velocidad;
    } else {
      throw new InvalidArgumentException("La velocidad debe ser un número.");
    }
  }

  /* ********************************************************************************* */
  /*                                     METHODS                                       */
  /* ********************************************************************************* */
  /*
   * Save DatoInstantaneo on persistent storage
   */
  public function saveDato()
  {
    $queryDatos = db_insert('rjsim_datos_partida')
      ->fields(array('id_partida', 'instante', 'posicion_x', 'posicion_y', 'posicion_z', 'velocidad', 'rpm', 'marcha'));
    $queryDatos->values($this->convertPropertiesToArrayForInsert());
    $queryDatos->execute();
  }

  /*
   * Return array of the same properties that are stored in DB
   */
  private function convertPropertiesToArrayForInsert()
  {
    $resultado = get_object_vars($this);
    unset($resultado['nombre_infraccion']);
    return $resultado;
  }

  /*
   * Return array of the properties that are going to be returned for Web Services
   */
  public function convertPropertiesToArrayForServices()
  {
    return get_object_vars($this);
  }
}