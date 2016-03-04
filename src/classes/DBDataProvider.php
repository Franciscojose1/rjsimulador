<?php

class DBDataProvider implements DataProvider {
  /* @var DBDataProvider */
  private static $provider;

  /**
   * Constructor privado para evitar instanciaciones externas de la clase.
   */
  private function __construct() {
  }

  /**
   * Singleton pattern
   * @return DBDataProvider Devuelve la única instancia del Provider.
   */
  public static function getInstance() {
    if (self::$provider == NULL) {
      self::$provider = new DBDataProvider();
    }
    return self::$provider;
  }

  /**
   * @inheritdoc
   */
  public function loadSimulatorUser($uid = null) {
    if (isset($uid)) {
      $user = user_uid_optional_load($uid);
    } else {
      $user = user_uid_optional_load();
    }

    if (!$user) {
      throw new LogicException("El usuario con el UID " . $uid . " no existe.");
    }

    return new UsuarioSimulacion($user);
  }

  /**
   * @inheritdoc
   */
  public function loadAllSimulatorUsers() {
    error_log("Entra loadAllSimulatorUsers");
    $query = db_select('rjsim_partida', 'p');
    $query->fields('p', array('uid'))
          ->distinct();
    $resultados = $query->execute();

    $usersUids = $resultados->fetchCol(0);

    $listaUsuarios = new ListaUsuariosSimulacion();
    foreach ($usersUids as $uid) {
      $usuario = $this->loadSimulatorUser($uid);
      $listaUsuarios->add($usuario);
    }

    return $listaUsuarios;
  }

  /**
   * @inheritdoc
   */
  public function loadAllTiposSimulaciones() {
    error_log("Entra");
    $query = db_select('rjsim_simulacion', 's');
    $query->fields('s', array('id_simulacion', 'nombre_simulacion'));

    $resultados = $query->execute();

    $idsSimulaciones = array();
    while ($resultado = $resultados->fetchAssoc()) {
      $idsSimulaciones[$resultado['id_simulacion']] = $resultado['nombre_simulacion'];
    }

    return $idsSimulaciones;
  }

  /**
   * @inheritdoc
   */
  public function loadAllTiposInfracciones() {
    $query = db_select('rjsim_infracciones', 'i');
    $query->fields('i', array('id_infraccion', 'nombre_infraccion'));
    $resultados = $query->execute();

    $idsInfracciones = array();
    while ($resultado = $resultados->fetchAssoc()) {
      $idsInfracciones[$resultado['id_infraccion']] = $resultado['nombre_infraccion'];
    }

    return $idsInfracciones;
  }

  /**
   * @inheritdoc
   */
  public function loadListaSimulacionesByUsuario(UsuarioSimulacion $usuario) {
    $idsSimulaciones = Constants::getTiposSimulacion();

    $listaSimulaciones = new ListaSimulaciones();
    foreach($idsSimulaciones as $id=>$nombre_simulacion) {
      $listaSimulaciones->add(new Simulacion($id, $usuario));
    }

    return $listaSimulaciones;
  }


  /**
   * @inheritdoc
   */
  public function loadListaPartidasBySimulation(Simulacion $simulation) {
    $listaPartidas = new ListaPartidas();

    $query = db_select('rjsim_partida', 'p');
    $query->fields('p', array('id_partida'))
      ->condition('uid', $simulation->getUsuarioSimulacionUid(), '=')
      ->condition('id_simulacion', $simulation->getIdSimulacion(), '=');

    $resultado = $query->execute();

    while ($record = $resultado->fetchAssoc()) {
      $listaPartidas->add(Partida::loadById($record['id_partida']));
    }

    return $listaPartidas;
  }

  /**
   * @inheritdoc
   */
  public function loadPartidaById($id_partida) {
    $partida = NULL;

    $query = db_select('rjsim_partida', 'p');
    $query->fields('p', array(
      'uid',
      'fecha',
      'id_simulacion',
      'consumo_medio',
      'consumo_total',
      'tiempo_total'
    ))
      ->condition('id_partida', $id_partida, '=');
    $resultado = $query->execute();

    if ($resultado->rowCount() == 0) {
      throw new Exception("No existe una partida con ese ID.");
    }

    while ($record = $resultado->fetchAssoc()) {
      $partida = new Partida($record['uid'], $record['fecha'], $record['id_simulacion']);
      $partida->setIdPartida($id_partida);
      $partida->setConsumoMedio($record['consumo_medio']);
      $partida->setConsumoTotal($record['consumo_total']);
      $partida->setTiempoTotal($record['tiempo_total']);
    }

    return $partida;
  }

  /**
   * @inheritdoc
   */
  public function loadListaInfraccionesByPartida(Partida $partida) {
    $listaInfracciones = new ListaInfracciones();

    $query = db_select('rjsim_infracciones_partida', 'ip');
    $query->fields('ip', array(
      'instante',
      'id_infraccion',
      'posicion_x',
      'posicion_y',
      'posicion_z',
      'observaciones'
    ))
      ->condition('ip.id_partida', $partida->getIdPartida(), '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $infraccion = new Infraccion($resultado['instante'], $resultado['id_infraccion']);
        $infraccion->setIdPartida($partida->getIdPartida());
        $infraccion->setPosicionX($resultado['posicion_x']);
        $infraccion->setPosicionY($resultado['posicion_y']);
        $infraccion->setPosicionZ($resultado['posicion_z']);
        $infraccion->setObservaciones($resultado['observaciones']);
        $listaInfracciones->add($infraccion);
      }
    }

    return $listaInfracciones;
  }

  /**
   * @inheritdoc
   */
  public function loadListaDatosByPartida(Partida $partida) {
    $listaDatos = new ListaDatosInstantaneos();

    $query = db_select('rjsim_datos_partida', 'dp');
    $query->fields('dp', array(
      'instante',
      'posicion_x',
      'posicion_y',
      'posicion_z',
      'velocidad',
      'rpm',
      'marcha',
      'consumo_instantaneo',
      'consumo_total'
    ))
      ->condition('dp.id_partida', $partida->getIdPartida(), '=');
    $resultados = $query->execute();

    if ($resultados->rowCount() > 0) {
      while ($resultado = $resultados->fetchAssoc()) {
        $dato = new DatoInstantaneo($resultado['instante'], $resultado['velocidad'], $resultado['rpm'], $resultado['marcha']);
        $dato->setIdPartida($partida->getIdPartida());
        $dato->setPosicion(array(
          'x' => $resultado['posicion_x'],
          'y' => $resultado['posicion_y'],
          'z' => $resultado['posicion_z']
        ));
        $dato->setConsumoInstantaneo($resultado['consumo_instantaneo']);
        $dato->setConsumoTotal($resultado['consumo_total']);
        $listaDatos->add($dato);
      }
    }

    return $listaDatos;
  }
}