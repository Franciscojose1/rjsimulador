<?php

class Infraccion implements ServicesAdapterInterface
{
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
  function __construct($instante, $id_infraccion)
  {
    $this->setInstante($instante);
    $this->setIdInfraccion($id_infraccion);
    $this->loadNombreInfraccion();
  }

  /* ********************************************************************************* */
  /*                                      ACCESSORS                                    */
  /* ********************************************************************************* */
  /**
   * @return float
   */
  public function getInstante()
  {
    return $this->instante;
  }

  /**
   * @param float $instante
   * @throws InvalidArgumentException
   */
  private function setInstante($instante)
  {
    if (is_float($instante)) {
      $this->instante = $instante;
    } else {
      throw new InvalidArgumentException("El instante de la infracción debe ser un número.");
    }
  }

  /**
   * @return int
   */
  public function getIdInfraccion()
  {
    return $this->id_infraccion;
  }

  /**
   * @param int $id_infraccion
   * @throws InvalidArgumentException
   */
  private function setIdInfraccion($id_infraccion)
  {
    if (is_numeric($id_infraccion)) {
      $this->id_infraccion = $id_infraccion;
    } else {
      throw new InvalidArgumentException("El ID de la infracción debe ser un entero.");
    }
  }

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
      throw new InvalidArgumentException("El ID de la partida debe ser un entero.");
    }
  }

  /**
   * @return string
   */
  public function getNombreInfraccion()
  {
    return $this->nombre_infraccion;
  }

  /**
   * @param string $nombre_infraccion
   */
  public function setNombreInfraccion($nombre_infraccion)
  {
    $this->nombre_infraccion = $nombre_infraccion;
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
   * @return array $posicion Position as array keys [x], [y] y [z]
   */
  public function getPosicion()
  {
    $posicion = array('x' => $this->getPosicionX(), 'y' => $this->getPosicionY(), 'z' => $this->getPosicionZ());
    return $posicion;
  }

  /**
   * @param array $posicion Position as array keys [x], [y] y [z]
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
   * @return string
   */
  public function getObservaciones()
  {
    return $this->observaciones;
  }

  /**
   * @param string $observaciones
   */
  public function setObservaciones($observaciones)
  {
    $this->observaciones = $observaciones;
  }

  /* ********************************************************************************* */
  /*                                       METHODS                                     */
  /* ********************************************************************************* */
  /*
   * Load nombre_infraccion from id_infraccion
   * @throws Exception
   */
  private function loadNombreInfraccion()
  {
    $query = db_select('rjsim_infracciones', 's')
      ->fields('s', array('nombre_infraccion'))
      ->condition('id_infraccion', $this->getIdInfraccion(), '=');
    $resultado = $query->execute();

    if ($resultado->rowCount() == 0) {
      throw new Exception("No existe una infracción con ese ID en la tabla de almacenamiento.");
    }

    while ($record = $resultado->fetchAssoc()) {
      $this->setNombreInfraccion($record['nombre_infraccion']);
    }
  }

  /*
   * Save Infraccion on persistent storage
   * @throws Exception
   */
  public function saveInfraccion()
  {
    if (!isset($this->id_partida) || !isset($this->id_infraccion) || !isset($this->instante)) {
      throw new Exception("Los campos ID de Partida, Instante e ID de Infracción son necesarios para almacenar una nueva Infraccion");
    }

    $queryInfracciones = db_insert('rjsim_infracciones_partida')
      ->fields(array('id_partida', 'instante', 'id_infraccion', 'posicion_x', 'posicion_y', 'posicion_z', 'observaciones'))
      ->values($this->convertPropertiesToArrayForInsert())
      ->execute();
  }

  /*
   * @return array Array of the same properties that are stored in DB
   */
  private function convertPropertiesToArrayForInsert()
  {
    $resultado = get_object_vars($this);
    unset($resultado['nombre_infraccion']);
    return $resultado;
  }

  /*
   * @return array Array of the properties that are going to be returned for Web Services
   */
  public function convertPropertiesToArrayForServices()
  {
    return get_object_vars($this);
  }
}