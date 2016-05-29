<?php

/**
 * Interface DataRemover Interfaz que deben implementar las clases para eliminar datos.
 */
interface DataRemover {
  /**
   * Elimina una partida de forma persistente.
   *
   * @param Partida $partida El objeto a eliminar.
   * @throws Exception Si ocurre un error eliminando la partida.
   */
  public function removePartida(Partida $partida);

  /**
   * Elimina una infracción de forma persistente.
   *
   * @param Infraccion $infraccion El objeto a eliminar.
   * @throws Exception Si ocurre un error eliminando la Infraccion.
   */
  public function removeInfraccion(Infraccion $infraccion);

  /**
   * Elimina un dato de forma persistente.
   *
   * @param DatoInstantaneo $dato El objeto a eliminar.
   * @throws Exception Si ocurre un error eliminando el DatoInstantaneo.
   */
  public function removeDatoInstantaneo(DatoInstantaneo $dato);
} 