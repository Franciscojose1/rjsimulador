<?php

interface DataProvider
{
  /**
   * Permite recuperar todos los ids y descipciones de las infracciones actuales.
   * @return array Array indexado de la forma Id_infraccion => Descripción de la infracción.
   */
  public function getAllIdsInfracciones();

  /**
   * Recupera el nombre asociado a un id de simulación.
   * @param int $id_simulacion El id de la simulación.
   * @return string Devuelve el nombre de la simulación asociado a ese ID.
   * @throws NoSuchElementException Si no existe esa simulación.
   */
  public function getNombreSimulacionFromId($id_simulacion);

  /**
   * @param Simulacion $simulation La simulación.
   * @return ListaPartidas Lista de partidas para la simulacion dada.
   */
  public function loadListaPartidasBySimulation(Simulacion $simulation);

  /**
   * @param int $id_partida El id de la partida.
   * @return Partida Partida con ese id instanciada con los datos almacenados de la misma.
   * @throws NoSuchElementException Si no existe una partida con ese id.
   */
  public function loadPartidaById($id_partida);

  /**
   * Recupera la lista de infracciones de una partida.
   * @param int $id_partida La partida de la que se van a recuperar las infracciones.
   * @return ListaInfracciones Lista de infracciones de la partida con ese id. Lista vacía si no encuentra ninguna.
   */
  public function loadListaInfraccionesByPartida(Partida $partida);

  /**
   * Recupera la lista de datos de una partida.
   * @param Partida $id_partida La partida de la que se van a recuperar los datos.
   * @return ListaDatosInstantaneos Lista de datos de la partida con ese id. Lista vacía si no encuentra ninguna.
   */
  public function loadListaDatosByPartida(Partida $partida);
} 