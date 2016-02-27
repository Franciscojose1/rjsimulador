<?php


class ListaUsuariosDataRetriever {

  /* @var ListaUsuariosSimulacion Una lista de usuarios de la que recuperar datos */
  private $listaUsuarios;
  /* @var array $arraySimulaciones Array de la forma $id=>$nombre_simulacion */
  private $arraySimulaciones;

  public function __construct(ListaUsuariosSimulacion $lista) {
    $provider = FactoryDataProvider::createDataProvider();
    // Recuperamos los ids de las simulaciones existentes
    $this->setArraySimulaciones($provider->loadAllIdsSimulaciones());
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
   * @return array
   */
  public function getArraySimulaciones() {
    return $this->arraySimulaciones;
  }

  /**
   * @param array $arraySimulaciones
   */
  private function setArraySimulaciones(array $arraySimulaciones) {
    $this->arraySimulaciones = $arraySimulaciones;
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
}