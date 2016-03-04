<?php

class DBDataSaver implements DataSaver {
  /* @var DBDataSaver */
  private static $saver;

  /**
   * Constructor privado para evitar instanciaciones externas de la clase.
   */
  private function __construct() {
  }

  /**
   * Singleton pattern
   * @return DBDataSaver Devuelve la única instancia del Saver.
   */
  public static function getInstance() {
    if (self::$saver == NULL) {
      self::$saver = new DBDataSaver();
    }
    return self::$saver;
  }

  /**
   * @inheritdoc
   */
  public function savePartida(Partida $partida) {
    $idPartidaNuevo = db_insert('rjsim_partida')
      ->fields(array(
        'uid' => $partida->getUserUid(),
        'fecha' => $partida->getFecha(),
        'id_simulacion' => $partida->getIdSimulacion(),
        'consumo_medio' => $partida->getConsumoMedio(),
        'consumo_total' => $partida->getConsumoTotal(),
        'tiempo_total' => $partida->getTiempoTotal()
      ))
      ->execute();

    $partida->setIdPartida($idPartidaNuevo);

    // Solo insertamos infracciones si existen
    if ($partida->getListaInfracciones() != NULL && $partida->getListaInfracciones()->count() > 0) {
      foreach ($partida->getListaInfracciones() as $infraccion) {
        $infraccion->setIdPartida($partida->getIdPartida());
        $infraccion->save();
      }
    }

    // Solo insertamos datos si existen
    if ($partida->getListaDatos() != NULL && $partida->getListaDatos()->count() > 0) {
      foreach ($partida->getListaDatos() as $dato) {
        $dato->setIdPartida($partida->getIdPartida());
        $dato->save();
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function saveInfraccion(Infraccion $infraccion) {
    $queryInfracciones = db_insert('rjsim_infracciones_partida')
      ->fields(array(
        'id_partida' => $infraccion->getIdPartida(),
        'instante' => $infraccion->getInstante(),
        'id_infraccion' => $infraccion->getIdInfraccion(),
        'posicion_x' => $infraccion->getPosicionX(),
        'posicion_y' => $infraccion->getPosicionY(),
        'posicion_z' => $infraccion->getPosicionZ(),
        'observaciones' => $infraccion->getObservaciones()
      ));

    $queryInfracciones->execute();
  }

  /**
   * @inheritdoc
   */
  public function saveDatoInstantaneo(DatoInstantaneo $dato) {
    $queryDatos = db_insert('rjsim_datos_partida')
      ->fields(
        array(
          'id_partida' => $dato->getIdPartida(),
          'instante' => $dato->getInstante(),
          'posicion_x' => $dato->getPosicionX(),
          'posicion_y' => $dato->getPosicionY(),
          'posicion_z' => $dato->getPosicionZ(),
          'velocidad' => $dato->getVelocidad(),
          'rpm' => $dato->getRpm(),
          'marcha' => $dato->getMarcha(),
          'consumo_instantaneo' => $dato->getConsumoInstantaneo(),
          'consumo_total' => $dato->getConsumoTotal()
        )
      );

    $queryDatos->execute();
  }
} 