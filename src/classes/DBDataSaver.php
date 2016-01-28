<?php

class DBDataSaver implements DataSaver
{
  /* @var DBDataSaver */
  private static $saver;

  /**
   * Constructor privado para evitar instanciaciones externas de la clase.
   */
  private function __construct()
  {
  }

  /**
   * Singleton pattern
   * @return DBDataSaver Devuelve la única instancia del Saver.
   */
  public static function getInstance()
  {
    if (self::$saver == null) {
      self::$saver = new DBDataSaver();
    }
    return self::$saver;
  }

  /**
   * Almacena una partida de forma persistente.
   * @param Partida $partida El objeto a almacenar.
   * @throws Exception Si se produce algún error almacenando la partida.
   */
  public function saveCompletePartida(Partida $partida)
  {
    // Create a transaction; if we catch exception we rollback
    $transaction = db_transaction();
    try {
      error_log(print_r($partida,TRUE));
      if ($partida->getUid() == null || $partida->getFecha() == null || $partida->getIdSimulacion() == null) {
        throw new Exception("Los campos UID, Fecha e ID de Simulación son necesarios para insertar una nueva partida");
      }

      $queryPartida = db_insert('rjsim_partida')
        ->fields(array(
          'uid' => $partida->getUid(),
          'fecha' => $partida->getFecha(),
          'id_simulacion' => $partida->getIdSimulacion(),
          'consumo_medio' => $partida->getConsumoMedio(),
          'consumo_total' => $partida->getConsumoTotal(),
          'tiempo_total' => $partida->getTiempoTotal()
        ));

      // Insertamos los datos de la partida para que se nos genere un id nuevo.
      $idPartidaNuevo = $queryPartida->execute();
      $partida->setIdPartida($idPartidaNuevo);

      // Solo insertamos infracciones si existen.
      if ($partida->getListaInfracciones() != null && $partida->getListaInfracciones()->count() > 0) {
        // Generamos la query de infracciones
        $queryInfracciones = db_insert('rjsim_infracciones_partida')
          ->fields(array('id_partida', 'instante', 'id_infraccion', 'posicion_x', 'posicion_y', 'posicion_z', 'observaciones'));

        foreach ($partida->getListaInfracciones() as $infraccion) {
          $infraccion->setIdPartida($partida->getIdPartida());
          if ($infraccion->getIdPartida() == null || $infraccion->getIdInfraccion() == null || $infraccion->getInstante() == null) {
            throw new Exception("Los campos ID de Partida, Instante e ID de Infracción son necesarios para almacenar una nueva Infraccion");
          }
          $queryInfracciones->values($infraccion->convertPropertiesToArrayForInsert());
        }
      }

      // Solo insertamos datos si existen.
      if ($partida->getListaDatos() != null && $partida->getListaDatos()->count() > 0) {
        $queryDatos = db_insert('rjsim_datos_partida')
          ->fields(array('id_partida', 'instante', 'posicion_x', 'posicion_y', 'posicion_z', 'velocidad', 'rpm', 'marcha',
            'consumo_instantaneo', 'consumo_total'));

        foreach ($partida->getListaDatos() as $dato) {
          $dato->setIdPartida($partida->getIdPartida());
          if ($dato->getIdPartida() == null || $dato->getInstante() == null) {
            throw new Exception("Los campos ID de Partida, Instante son necesarios para almacenar un nuevo Dato.");
          }
          $queryDatos->values($dato->convertPropertiesToArrayForInsert());
        }
      }

      if (isset($queryInfracciones)) {
        $queryInfracciones->execute();
      }

      if (isset($queryDatos)) {
        $queryDatos->execute();
      }

      // Commit unsetting $transaction variable
      unset($transaction);
    } catch (Exception $e) {
      $transaction->rollback();
      throw $e;
    }
  }

  /**
   * Almacena una partida de forma persistente.
   * @param Partida $partida El objeto a almacenar.
   * @throws Exception Si se produce algún error almacenando la partida.
   */
  public function savePartida(Partida $partida)
  {
    // Creamos una transacción; si algo falla hacemos rollback
    $transaction = db_transaction();
    try {
      $idPartidaNuevo = db_insert('rjsim_partida')
        ->fields(array(
          'uid' => $partida->getUid(),
          'fecha' => $partida->getFecha(),
          'id_simulacion' => $partida->getIdSimulacion(),
          'consumo_medio' => $partida->getConsumoMedio(),
          'consumo_total' => $partida->getConsumoTotal(),
          'tiempo_total' => $partida->getTiempoTotal()
        ))
        ->execute();

      $partida->setIdPartida($idPartidaNuevo);

      // Solo insertamos infracciones si existen
      if ($partida->getListaInfracciones() != null && $partida->getListaInfracciones()->count() > 0) {
        foreach ($partida->getListaInfracciones() as $infraccion) {
          $infraccion->setIdPartida($partida->getIdPartida());
          $infraccion->save();
        }
      }

      // Solo insertamos datos si existen
      if ($partida->getListaDatos() != null && $partida->getListaDatos()->count() > 0) {
        foreach ($partida->getListaDatos() as $dato) {
          $dato->setIdPartida($partida->getIdPartida());
          $dato->save();
        }
      }

      // Hacemos commit deseteando la variable $transaction
      unset($transaction);

    } catch (Exception $e) {
      $transaction->rollback();
      throw $e;
    }
  }

  public function saveInfraccion(Infraccion $infraccion)
  {
    $queryInfracciones = db_insert('rjsim_infracciones_partida')
      ->fields(array('id_partida', 'instante', 'id_infraccion', 'posicion_x', 'posicion_y', 'posicion_z', 'observaciones'));
    $queryInfracciones->values($infraccion->convertPropertiesToArrayForInsert());
    $queryInfracciones->execute();
  }

  public function saveDatoInstantaneo(DatoInstantaneo $dato)
  {
    $queryDatos = db_insert('rjsim_datos_partida')
      ->fields(array('id_partida', 'instante', 'posicion_x', 'posicion_y', 'posicion_z', 'velocidad', 'rpm', 'marcha',
        'consumo_instantaneo', 'consumo_total'));
    $queryDatos->values($dato->convertPropertiesToArrayForInsert());
    $queryDatos->execute();
  }
} 