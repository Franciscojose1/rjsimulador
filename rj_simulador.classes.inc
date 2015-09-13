<?php

class Partida
{
  public $id_partida;
  public $uid;
  public $fecha;
  public $id_simulacion;
  public $nombre_simulacion;
  public $infracciones;  // Array de infracciones
  public $datos;      // Array de datos

  /*
    Constructor Partida
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
    Constructor -> Retrieve basic data of a certain Partida from Database
    @param int id_partida -> Partida ID
  */
  public function __construct1($id_partida)
  {
    $query = db_select('rjsim_partida', 'p');
    $query->leftJoin('rjsim_simulacion', 's', 'p.id_simulacion = s.id_simulacion');
    $query->fields('p', array('uid', 'fecha', 'id_simulacion'))
      ->fields('s', array('nombre_simulacion'))
      ->condition('id_partida', $id_partida, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $this->id_partida = $id_partida;
      $this->uid = $record['uid'];
      $this->fecha = $record['fecha'];
      $this->id_simulacion = $record['id_simulacion'];
      $this->nombre_simulacion = $record['nombre_simulacion'];
    }
  }

  /*
   * Constructor -> Retrieve a Partida from Database with datos and/or infracciones
   * @param int id_partida -> Partida ID
   * @param string datos_a_recuperar -> 'datos', 'infracciones', 'todos'
   */
  public function __construct2($id_partida, $datos_a_recuperar)
  {
    self::__construct1($id_partida);
    switch ($datos_a_recuperar) {
      case 'infracciones':
        $this->retrieveInfraccionesFromDatabase();
        break;
      case 'datos':
        $this->retrieveDatosFromDatabase();
        break;
      case 'todos':
        $this->retrieveInfraccionesFromDatabase();
        $this->retrieveDatosFromDatabase();
    }

  }

  public function insertInDatabase()
  {
    $id_partida = db_insert('rjsim_partida')
      ->fields(array(
        'uid' => $this->uid,
        'fecha' => $this->fecha,
        'id_simulacion' => $this->id_simulacion,
      ))
      ->execute();
    $this->id_partida = $id_partida;

    // If not exists infracciones, don't insert anything
    if (!empty($this->infracciones)) {
      $queryInfracciones = db_insert('rjsim_infracciones_partida')
        ->fields(array('id_partida', 'instante', 'id_infraccion'));
      foreach ($this->infracciones as $infraccion) {
        $infraccion['id_partida'] = $this->id_partida;
        $queryInfracciones->values($infraccion);
      }
      $queryInfracciones->execute();
    }

    // If not exists datos, don't insert anything
    if (!empty($this->datos)) {
      $queryDatos = db_insert('rjsim_datos_partida')
        ->fields(array('id_partida', 'instante', 'velocidad', 'revoluciones', 'marcha'));
      foreach ($this->datos as $dato) {
        $dato['id_partida'] = $this->id_partida;
        $queryDatos->values($dato);
      }
      $queryDatos->execute();
    }
  }

  public function retrieveInfraccionesFromDatabase()
  {
    $query = db_select('rjsim_infracciones_partida', 'ip');
    $query->leftJoin('rjsim_infracciones', 'i', 'ip.id_infraccion = i.id_infraccion');
    $query->fields('ip', array('instante', 'id_infraccion'))
      ->fields('i', array('nombre_infraccion'))
      ->condition('ip.id_partida', $this->id_partida, '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $this->infracciones[] = $resultado;
      }
    }
  }

  public function retrieveDatosFromDatabase()
  {
    $query = db_select('rjsim_datos_partida', 'dp');
    $query->fields('dp', array('instante', 'velocidad', 'revoluciones', 'marcha'))
      ->condition('dp.id_partida', $this->id_partida, '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $this->datos[] = $resultado;
      }
    }
  }
}

class Simulacion
{
  public $id_simulacion;
  public $nombre_simulacion;
  public $velocidad_media;
  public $revoluciones_medias;

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
        self::__construct2($argv[0],$argv[1]);
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
      $this->velocidad_media['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->revoluciones_medias['revoluciones_medias'] = $this->retrieveAverageData('revoluciones');
    }
  }

  /*
    Constructor -> Retrieve basic data of a certain Simulacion from Database
    @param int id_partida -> Simulacion ID
  */
  public function __construct2($id_simulacion,$uid)
  {
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('nombre_simulacion'))
      ->condition('id_simulacion', $id_simulacion, '=');
    $resultado = $query->execute();
    while ($record = $resultado->fetchAssoc()) {
      $this->id_simulacion = $id_simulacion;
      $this->nombre_simulacion = $record['nombre_simulacion'];
      $this->velocidad_media['velocidad_media'] = $this->retrieveAverageData('velocidad');
      $this->revoluciones_medias['revoluciones_medias'] = $this->retrieveAverageData('revoluciones');
      if($uid > 0){
        $this->velocidad_media['velocidad_media_usuario'] = $this->retrieveAverageData('velocidad',$uid);
        $this->revoluciones_medias['revoluciones_medias_usuario'] = $this->retrieveAverageData('revoluciones',$uid);
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