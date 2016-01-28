<?php

interface DataSaver
{
  /**
   * Almacena una partida de forma persistente.
   * @param Partida $partida El objeto a almacenar.
   */
  public function savePartida(Partida $partida);

  /**
   * Almacena un infracción de forma persistente.
   * @param Infraccion $infraccion El objeto a almacenar.
   */
  public function saveInfraccion(Infraccion $infraccion);

  /**
   * Almacena un dato de forma persistente.
   * @param DatoInstantaneo $dato El objeto a almacenar.
   */
  public function saveDatoInstantaneo(DatoInstantaneo $dato);
} 