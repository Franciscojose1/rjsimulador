<?php

interface DataProvider {
  /**
   * Recupera un usuario de la BBDD si tiene alguna partida.
   * @param int|null $uid El UID del usuario a cargar. O NULL para cargar al usuario actual.
   * @return UsuarioSimulacion Un objeto Usuario cargado.
   * @throws LogicException Si no existe un usuario con ese UID en la BBDD.
   */
  public function loadSimulatorUser($uid = null);

  /**
   * Recupera todos los usuarios que tienen alguna partida en el simualdor.
   * @return ListaUsuariosSimulacion Lista de los usuarios que tienen alguna partida en el simulador.
   */
  public function loadAllSimulatorUsers();

  /**
   * Permite recuperar todos los ids y descripciones de las infracciones actuales.
   * @return array Array indexado de la forma Id_infraccion => descripción de la infracción.
   */
  public function loadAllTiposInfracciones();

  /**
   * Permite recuperar todos los ids y descripciones de las simulaciones actuales.
   * @return array Array indexado de la forma id_simulacion => descripción de la simulación.
   */
  public function loadAllTiposSimulaciones();

  /**
   * Devuelve la lista con todas las simulaciones de este usuario.
   * @param int $userUid El uid del usuario.
   * @return ListaSimulaciones La lista de las simulaciones para este usuario.
   */
  public function loadListaSimulacionesByUsuario($userUid);

  /**
   * @param Simulacion $simulation La simulación.
   * @return ListaPartidas Lista de partidas para la simulacion dada.
   */
  public function loadListaPartidasBySimulation(Simulacion $simulation);

  /**
   * @param int $id_partida El id de la partida.
   * @return Partida Partida con ese id instanciada con los datos almacenados de la misma.
   * @throws Exception Si no existe una partida con ese id.
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