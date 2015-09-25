<?php

class Infraccion implements ServicesAdapterInterface
{
  /* ********************************************************************************* */
  /*                                      PROPERTIES                                   */
  /* ********************************************************************************* */
  private $id_partida;
  private $instante;
  private $id_infraccion;
  private $nombre_infraccion;
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
   * @return int
   */
  public function getInstante()
  {
    return $this->instante;
  }

  /**
   * @param int $instante
   * @throws InvalidArgumentException
   */
  private function setInstante($instante)
  {
    if (is_numeric($instante)) {
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
   * @return mixed
   */
  public function getNombreInfraccion()
  {
    return $this->nombre_infraccion;
  }

  /**
   * @param mixed $nombre_infraccion
   */
  public function setNombreInfraccion($nombre_infraccion)
  {
    $this->nombre_infraccion = $nombre_infraccion;
  }

  /**
   * @return mixed
   */
  public function getObservaciones()
  {
    return $this->observaciones;
  }

  /**
   * @param mixed $observaciones
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
      ->fields(array('id_partida', 'instante', 'id_infraccion', 'observaciones'))
      ->values($this->convertPropertiesToArrayForInsert())
      ->execute();
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