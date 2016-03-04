<?php

class ListaUsuariosDataRetriever {

  /* @var ListaUsuariosSimulacion Una lista de usuarios de la que recuperar datos */
  private $listaUsuarios;

  public function __construct(ListaUsuariosSimulacion $lista) {
    // Recuperamos los ids de las simulaciones existentes
    $this->listaUsuarios = $lista;
  }

  /**
   * @return ListaUsuariosSimulacion
   */
  public function getListaUsuarios() {
    return $this->listaUsuarios;
  }

  /**
   * @param ListaUsuariosSimulacion $listaUsuarios
   */
  public function setListaUsuarios(ListaUsuariosSimulacion $listaUsuarios) {
    $this->listaUsuarios = $listaUsuarios;
  }

  /**
   * Método que devuelve todas las partidas de la lista.
   * @return ListaPartidas Listado de todas las partidas de los usuarios pasados.
   */
  public function retrieveAllPartidas() {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getListaUsuarios() as $usuario) {
      $listaPartidas->mergeList($usuario->retrieveAllPartidas());
    }

    return $listaPartidas;
  }

  /**
   * Método que devuelve todas las partidas de los usuarios de la lista para una simulación en concreto.
   * @param int $idSimulacion El id de la simulación para la que recuperar todas las partidas.
   * @return ListaPartidas Lista de todas las partidas de la simulacion para los usuarios de la lista.
   */
  public function retrieveAllPartidasByIdSimulacion($idSimulacion) {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getListaUsuarios() as $usuario) {
      $listaPartidas->mergeList($usuario->retrieveAllPartidasByIdSimulacion($idSimulacion));
    }

    return $listaPartidas;
  }

  /**
   * Devuelve todas las infracciones de todas las partidas de la lista de usuarios para una simulación en concreto.
   * @param int $idSimulacion ID de la Simulación para la que recuperar las infracciones.
   * @return \ListaInfracciones Lista de infracciones para la simulación pasada.
   */
  public function retrieveAllInfraccionesByIdSimulacion($idSimulacion) {
    $listaInfracciones = new ListaInfracciones();
    // Recuperamos todas las infracciones de una simulación en concreto
    foreach ($this->retrieveAllPartidasByIdSimulacion($idSimulacion) as $partida) {
      $listaInfracciones->mergeList($partida->getListaInfracciones());
    }

    return $listaInfracciones;
  }

  /**
   * Devuelve todas las infracciones de un cierto tipo de la lista de ususarios para las simulación pasada.
   * @param int $idInfraccion El ID de la infracción a recuperar.
   * @param int $idSimulacion ID de la simulación para la que recuperar las infracciones.
   * @return \ListaInfracciones Lista de infracciones del tipo pasado para la simulación.
   */
  public function retrieveAllInfraccionesByTypeAndIdSimulacion($idInfraccion, $idSimulacion) {
    $listaInfracciones = $this->retrieveAllInfraccionesByIdSimulacion($idSimulacion);
    $arrayIdsInfracciones = array($idInfraccion);
    // Devolvemos la lista de infracciones filtradas por el IdInfraccion
    return $listaInfracciones->filterBy(new FilterByEquality($arrayIdsInfracciones, FilterByEquality::INFRACCION_ID));
  }

  /**
   * Devuelve la media de infracciones de un cierto tipo cometidas por partida de lal lista de usuarios para la simulación pasada.
   * @param int $idInfraccion El ID de la infracción.
   * @param int $idSimulation El ID dela simulación.
   * @return float|int Media de infracciones por partida del grupo de usuarios.
   */
  public function getAverageInfraccionesByPartida($idInfraccion, $idSimulation) {
    // Recuperamos total de partidas de una simulacion
    $totalPartidasSimulacion = $this->retrieveAllPartidasByIdSimulacion($idSimulation)
      ->count();
    // Recuperamos total de infracciones de un ciertto tipo por simulacion
    $totalInfraccionesSimulacion = $this->retrieveAllInfraccionesByTypeAndIdSimulacion($idInfraccion, $idSimulation)
      ->count();

    // Si el total de partidas es 0 devolvemos 0 evitando la división
    $mediaInfraccionesPorPartida = $totalPartidasSimulacion > 0 ? $totalInfraccionesSimulacion / $totalPartidasSimulacion : 0;

    return $mediaInfraccionesPorPartida;
  }
}