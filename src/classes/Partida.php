<?php

class Partida implements ServicesAdapterInterface
{
  /*
 * Posibles valores para recuperar datos de una Partida
 */
  const PARTIDA_INFRACCIONES = 'infracciones';
  const PARTIDA_DATOS = 'datos';
  const PARTIDA_INFRACCIONES_DATOS = 'todos';
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
    $this->loadNombreSimulacion();
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
    $query->fields('p', array('uid', 'fecha', 'id_simulacion'))
      ->condition('id_partida', $id_partida, '=');
    $resultado = $query->execute();
    if ($resultado->rowCount() == 0) {
      throw new Exception("No existe una partida con ese ID.");
    }
    while ($record = $resultado->fetchAssoc()) {
      $partida = new Partida($record['uid'], $record['fecha'], $record['id_simulacion']);
      $partida->setIdPartida($id_partida);
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
  private function setIdPartida($id_partida)
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
  private function setNombreSimulacion($nombre_simulacion)
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
  public function addInfraccion(Infraccion $infraccion)
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
  public function addDato(DatoInstantaneo $dato)
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
   * Load nombre_simulacion from id_simulacion
   * @throws Exception
   */
  private function loadNombreSimulacion()
  {
    $query = db_select('rjsim_simulacion', 's')
      ->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $this->getIdSimulacion(), '=');
    $resultado = $query->execute();

    if ($resultado->rowCount() == 0) {
      throw new Exception("No existe una simulación con ese ID en la tabla de almacenamiento.");
    }

    while ($record = $resultado->fetchAssoc()) {
      $this->setNombreSimulacion($record['nombre_simulacion']);
    }
  }

  /*
   * Retrieve array of Infraccion from persistent storage
   */
  public function loadInfracciones()
  {
    $query = db_select('rjsim_infracciones_partida', 'ip');
    $query->fields('ip', array('instante', 'id_infraccion', 'observaciones'))
      ->condition('ip.id_partida', $this->id_partida, '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $infraccion = new Infraccion($resultado['instante'], $resultado['id_infraccion']);
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
        $dato->setPosicion(array('x' => $resultado['posicion_x'], 'y' => $resultado['posicion_y'], 'z' => $resultado['posicion_z']));
        $this->addDato($dato);
      }
    }
  }

  public function convertPropertiesToArrayForServices()
  {
    $partida = get_object_vars($this);
    foreach ($this->getInfracciones() as $key => $infraccion) {
      $partida['infracciones'][$key] = $infraccion->convertPropertiesToArrayForServices();
    }
    foreach ($this->getDatos() as $key => $dato) {
      $partida['datos'][$key] = $dato->convertPropertiesToArrayForServices();
    }
    return $partida;
  }
}