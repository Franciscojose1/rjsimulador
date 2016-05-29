<?php

/**
 * Class DBDataRemover Implementación de una clase para borrar datos en una BBDD.
 */
class DBDataRemover implements DataRemover {
  /* @var DBDataRemover */
  private static $remover;

  /**
   * Constructor privado para evitar instanciaciones externas de la clase.
   */
  private function __construct() {
  }

  /**
   * Singleton pattern
   * @return DBDataRemover Devuelve la única instancia del Remover.
   */
  public static function getInstance() {
    if (self::$remover == NULL) {
      self::$remover = new DBDataRemover();
    }
    return self::$remover;
  }

  /**
   * @inheritdoc
   */
  public function removePartida(Partida $partida) {
    // Cuando eliminamos una partida podemos realizar las eliminaciones en un solo DELETE
    // Si existen infracciones las eliminamos
    if ($partida->getListaInfracciones() != NULL && $partida->getListaInfracciones()->count() > 0) {
      db_delete('rjsim_infracciones_partida')
        ->condition('id_partida', $partida->getIdPartida(), '=')
        ->execute();
    }

    // Si existen datos los eliminamos
    if ($partida->getListaDatos() != NULL && $partida->getListaDatos()->count() > 0) {
      db_delete('rjsim_datos_partida')
        ->condition('id_partida', $partida->getIdPartida(), '=')
        ->execute();
    }

    // Eliminamos la partida
    db_delete('rjsim_partida')
      ->condition('id_partida', $partida->getIdPartida(), '=')
      ->execute();
  }

  /**
   * @inheritdoc
   */
  public function removeInfraccion(Infraccion $infraccion) {
    db_delete('rjsim_infracciones_partida')
      ->condition('id_partida', $infraccion->getIdPartida(), '=')
      ->condition('instante', $infraccion->getInstante(), '=')
      ->condition('id_infraccion', $infraccion->getIdInfraccion())
      ->execute();
  }

  /**
   * @inheritdoc
   */
  public function removeDatoInstantaneo(DatoInstantaneo $dato) {
    db_delete('rjsim_datos_partida')
      ->condition('id_partida', $dato->getIdPartida(), '=')
      ->condition('instante', $dato->getInstante(), '=')
      ->execute();
  }
} 