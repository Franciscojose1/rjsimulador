<?php

class Partida
{
  /* ********************************************************************************* */
  /*                                      PROPERTIES                                   */
  /* ********************************************************************************* */
  private $id_partida;
  private $uid;
  private $fecha;
  private $id_simulacion;
  private $nombre_simulacion;
  private $infracciones;  // Array de objetos InfraccionInstantanea
  private $datos;         // Array de objetos DatoInstantaneo

  /* ********************************************************************************* */
  /*                                     CONSTRUCTOR                                   */
  /* ********************************************************************************* */
  function __construct($uid, $fecha, $id_simulacion)
  {
    $this->setUid($uid);
    $this->setFecha($fecha);
    $this->setIdSimulacion($id_simulacion);
  }

  /*
   * Factory Constructor -> Retrieve a Partida from Database with datos and/or infracciones
   * @param int id_partida
   * @param string datos_a_recuperar -> 'datos', 'infracciones', 'todos', null
   * @return object Partida
   */
  public static function loadPartidaById($id_partida, $datos_a_recuperar = null)
  {
    $partida = null;

    $query = db_select('rjsim_partida', 'p');
    $query->leftJoin('rjsim_simulacion', 's', 'p.id_simulacion = s.id_simulacion');
    $query->fields('p', array('uid', 'fecha', 'id_simulacion'))
      ->fields('s', array('nombre_simulacion'))
      ->condition('id_partida', $id_partida, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $partida = self::__construct($record['uid'], $record['fecha'], $record['id_simulacion']);
      $partida->setIdPartida($id_partida);
      $partida->setNombreSimulacion($record['nombre_simulacion']);
    }

    if (isset($datos_a_recuperar)) {
      switch ($datos_a_recuperar) {
        case 'infracciones':
          $partida->loadInfracciones();
          break;
        case 'datos':
          $partida->loadDatos();
          break;
        case 'todos':
          $partida->loadInfracciones();
          $partida->loadDatos();
      }
    }
    return $partida;
  }

  /* ********************************************************************************* */
  /*                                      ACCESSORS                                    */
  /* ********************************************************************************* */
  /**
   * @return array
   */
  public function getDatos()
  {
    return $this->datos;
  }

  /**
   * @param array $datos
   * @throws InvalidArgumentException
   */
  public function setDatos($datos)
  {
    if (is_array($datos)) {
      foreach ($datos as $dato) {
        try {
          $this->addDato($dato);
        } catch (InvalidArgumentException $e) {
          throw $e;
        }
      }
    } else {
      throw new InvalidArgumentException("Los datos deben ser un array.");
    }
  }

  /**
   * @return int
   */
  public function getFecha()
  {
    return $this->fecha;
  }

  /**
   * @param int $fecha Fecha en tiempo UNIX
   * @throws InvalidArgumentException
   */
  public function setFecha($fecha)
  {
    if (is_numeric($fecha)) {
      $this->fecha = $fecha;
    } else {
      throw new InvalidArgumentException("La fecha se tiene que pasar convertida tiempo UNIX.");
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
      throw new InvalidArgumentException("El ID de la Partida debe ser un entero");
    }
  }

  /**
   * @return int
   */
  public function getIdSimulacion()
  {
    return $this->id_simulacion;
  }

  /**
   * @param int $id_simulacion
   * @throws InvalidArgumentException
   */
  public function setIdSimulacion($id_simulacion)
  {
    if (is_numeric($id_simulacion)) {
      $this->id_simulacion = $id_simulacion;
    } else {
      throw new InvalidArgumentException("El ID de la Simulación debe ser un entero.");
    }
  }

  /**
   * @return array
   */
  public function getInfracciones()
  {
    return $this->infracciones;
  }

  /**
   * @param array $infracciones
   * @throws InvalidArgumentException
   */
  public function setInfracciones($infracciones)
  {
    if (is_array($infracciones)) {
      foreach ($infracciones as $infraccion) {
        try {
          $this->addInfraccion($infraccion);
        } catch (InvalidArgumentException $e) {
          throw $e;
        }
      }
    } else {
      throw new InvalidArgumentException("Las infracciones deben ser un array.");
    }
  }

  /**
   * @return string
   */
  public function getNombreSimulacion()
  {
    return $this->nombre_simulacion;
  }

  /**
   * @param mixed $nombre_simulacion
   */
  public function setNombreSimulacion($nombre_simulacion)
  {
    $this->nombre_simulacion = $nombre_simulacion;
  }

  /**
   * @return int
   */
  public function getUid()
  {
    return $this->uid;
  }

  /**
   * @param int $uid
   * @throws InvalidArgumentException
   */
  public function setUid($uid)
  {
    if (is_numeric($uid)) {
      $this->uid = $uid;
    } else {
      throw new InvalidArgumentException("El UID tiene que ser un entero.");
    }
  }

  /* ********************************************************************************* */
  /*                                      METHODS                                      */
  /* ********************************************************************************* */
  /*
   * @param object $infraccion
   * @throws InvalidArgumentException
   */
  public function addInfraccion($infraccion)
  {
    if (is_object($infraccion)) {
      $this->infracciones[] = $infraccion;
    } else {
      throw new InvalidArgumentException("La infraccion a introducir debe ser un objeto de tipo Infraccion");
    }
  }

  /*
   * @param object $dato
   * @throws InvalidArgumentException
   */
  public function addDato($dato)
  {
    if (is_object($dato)) {
      $this->datos[] = $dato;
    } else {
      throw new InvalidArgumentException("El dato a introducir deber ser un objeto de tipo DatoInsatanteo");
    }
  }

  /*
   * Save Partida on persistent storage
   * @throws Exception
   */
  public function savePartida()
  {
    if (!isset($this->uid) || !isset($this->fecha) || !isset($this->id_simulacion)) {
      throw new Exception("Los campos UID, Fecha e ID de Simulación son necesarios para insertar una nueva partida");
    }

    $id_partida = db_insert('rjsim_partida')
      ->fields(array(
        'uid' => $this->getUid(),
        'fecha' => $this->getFecha(),
        'id_simulacion' => $this->getIdSimulacion(),
      ))
      ->execute();

    $this->setIdPartida($id_partida);

    // If not exists infracciones, don't insert anything
    if (!empty($this->infracciones)) {
      foreach ($this->infracciones as $infraccion) {
        $infraccion->setIdPartida($id_partida);
        $infraccion->saveInfraccion();
      }
    }

    // If not exists datos, don't insert anything
    if (!empty($this->datos)) {
      foreach ($this->datos as $dato) {
        $dato->setIdPartida($id_partida);
        $dato->saveDato();
      }
    }
  }

  /*
   * Retrieve array of Infraccion from persistent storage
   */
  public function loadInfracciones()
  {
    $query = db_select('rjsim_infracciones_partida', 'ip');
    $query->leftJoin('rjsim_infracciones', 'i', 'ip.id_infraccion = i.id_infraccion');
    $query->fields('ip', array('instante', 'id_infraccion', 'observaciones'))
      ->fields('i', array('nombre_infraccion'))
      ->condition('ip.id_partida', $this->id_partida, '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $infraccion = new Infraccion($resultado['id_infraccion'], $resultado['instante']);
        $infraccion->setIdPartida($this->getIdPartida());
        $infraccion->setObservaciones($resultado['observaciones']);
        $this->addInfraccion($infraccion);
      }
    }
  }

  /*
   * Retrieve array of DatoInstantaneo from persistent storage
   */
  public function loadDatos()
  {
    $query = db_select('rjsim_datos_partida', 'dp');
    $query->fields('dp', array('instante', 'posicion_x', 'posicion_y', 'posicion_z', 'velocidad', 'rpm', 'marcha'))
      ->condition('dp.id_partida', $this->id_partida, '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $dato = new DatoInstantaneo($resultado['instante'], $resultado['velocidad'], $resultado['rpm'], $resultado['marcha']);
        $dato->setIdPartida($this->getIdPartida());
        $this->addDato($dato);
      }
    }
  }
}

class Infraccion
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
  public function setInstante($instante)
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
    if(is_numeric($id_partida)){
    $this->id_partida = $id_partida;
    }
    else{
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
                        ->values($this->convertPropertiesToArray())
                        ->execute();
  }

  /*
   * Return array of the same properties that are stored in DB
   */
  public function convertPropertiesToArray()
  {
    $resultado = get_object_vars($this);
    unset($resultado['nombre_infraccion']);
    return $resultado;
  }
}

class DatoInstantaneo
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
    if(is_numeric($id_partida)){
      $this->id_partida = $id_partida;
    }
    else{
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
    if(is_numeric($instante)) {
      $this->instante = $instante;
    }
    else{
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
    if(is_numeric($rpm)) {
      $this->rpm = $rpm;
    }
    else{
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
    if(is_numeric($marcha)){
      $this->marcha = $marcha;
    }
    else{
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
    if(is_numeric($posicion_x)){
      $this->posicion_x = $posicion_x;
    }
    else{
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
    if(is_numeric($posicion_y)){
      $this->posicion_y = $posicion_y;
    }
    else{
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
    if(is_numeric($posicion_z)){
      $this->posicion_z = $posicion_z;
    }
    else{
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
    if(is_numeric($velocidad)) {
      $this->velocidad = $velocidad;
    }
    else{
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
    $queryDatos->values($this->convertPropertiesToArray());
    $queryDatos->execute();
  }

  /*
   * Return array of the same properties that are stored in DB
   */
  public function convertPropertiesToArray()
  {
    return get_object_vars($this);
  }
}

class Simulacion
{
  public $id_simulacion;
  public $nombre_simulacion;
  public $datos_medios;

  /*
     Constructor Simulacion
   */
  public function __construct()
  {
    $argv = func_get_args();
    switch (func_num_args()) {
      case 0:
        self::__construct0();
        break;
      case 1:
        self::__construct1($argv[0]);
        break;
      case 2:
        self::__construct2($argv[0], $argv[1]);
        break;
    }
  }

  /*
    Constructor Empty
  */
  public function __construct0()
  {
  }

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct1($id_simulacion)
  {
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $id_simulacion, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $this->id_simulacion = $id_simulacion;
      $this->nombre_simulacion = $record['nombre_simulacion'];
      $this->datos_medios['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->datos_medios['revoluciones_medias'] = $this->retrieveAverageData('revoluciones');
    }
  }

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct2($id_simulacion, $uid)
  {
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $id_simulacion, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $this->id_simulacion = $id_simulacion;
      $this->nombre_simulacion = $record['nombre_simulacion'];
      $this->datos_medios['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->datos_medios['revoluciones_medias'] = $this->retrieveAverageData('revoluciones');
      if ($uid > 0) {
        $this->datos_medios['velocidad_media_usuario'] = $this->retrieveAverageData('velocidad', $uid);
        $this->datos_medios['revoluciones_medias_usuario'] = $this->retrieveAverageData('revoluciones', $uid);
      }
    }
  }

  public function retrieveAverageData($parameter, $uid = null)
  {
    $query = db_select('rjsim_partida', 'p');
    $query->innerJoin('rjsim_datos_partida', 'dp', 'p.id_partida = dp.id_partida');
    $query->fields('p', array('id_partida'))
      ->condition('p.id_simulacion', $this->id_simulacion, '=');
    if (isset($uid) && $uid != null) {
      $query->condition('p.uid', $uid, '=');
    }
    $count = $query->execute()->rowCount();
    if ($count == 0) {
      return null;
    }

    $query = db_select('rjsim_partida', 'p');
    $query->innerJoin('rjsim_datos_partida', 'dp', 'p.id_partida = dp.id_partida');
    $query->addExpression('SUM(' . $parameter . ')', 'suma_total');
    $query->condition('p.id_simulacion', $this->id_simulacion, '=');
    if (isset($uid) && $uid != null) {
      $query->condition('p.uid', $uid, '=');
    }
    $records = $query->execute();

    $resultado = null;
    while ($record = $records->fetchAssoc()) {
      $resultado = $record['suma_total'] / $count;
    }

    return $resultado;
  }
}