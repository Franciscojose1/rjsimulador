<?php

class GestorSimulaciones {
  /* @var UsuarioSimulacion $usuarioActual EL usuario actual. */
  private $usuarioActual;
  /* @var ListaUsuariosSimulacion $listaTodosUsuarios Todos los usuarios que tienen partidas almacenadas de las Simulaciones. */
  private $listaTodosUsuarios;
  /* @var array $arraySimulaciones Array de la forma $id=>$nombre_simulacion */
  private $arraySimulaciones;

  /**
   * @param UsuarioSimulacion $usuarioActual El usuario actual.
   * @throws LogicException Si no existen usuarios en la BBD.
   */
  public function __construct(UsuarioSimulacion $usuarioActual = NULL) {
    $provider = FactoryDataProvider::createDataProvider();

    // Recuperamos los ids de las simulaciones existentes
    $this->setArraySimulaciones($provider->loadAllIdsSimulaciones());

    // Recuperamos todos los usuarios con partidas
    $listaDeTodosLosUsuarios = $provider->loadAllSimulatorUsers();

    if ($listaDeTodosLosUsuarios->count() == 0) {
      throw new LogicException("Ningún usuario ha guardado partidas en la BBDD.");
    }

    $this->setListaTodosUsuarios($listaDeTodosLosUsuarios);

    // Comprobamos si estamos usando un usuario específico
    if (isset($usuarioActual)) {
      $this->setUsuarioActual($usuarioActual);
    }
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
   * @return UsuarioSimulacion
   * @throws Exception Si no se ha pasado un usuario al constructor del GestorSimulaciones.
   */
  public function getUsuarioActual() {
    if (!isset($this->usuarioActual)) {
      throw new LogicException("No se ha pasado un usuario como usuario actual.");
    }

    return $this->usuarioActual;
  }

  /**
   * @param UsuarioSimulacion $usuarioActual
   * @throws LogicException Si el usuario no tiene partidas guardadas.
   */
  public function setUsuarioActual(UsuarioSimulacion $usuarioActual) {
    if ($usuarioActual->countPartidas() > 0) {
      $this->usuarioActual = $usuarioActual;
    } else {
      throw new LogicException("No existen partidas guardadas en la BBDD.");
    }
  }

  /**
   * @return ListaUsuariosSimulacion
   */
  public function getListaTodosUsuarios() {
    return $this->listaTodosUsuarios;
  }

  /**
   * @param ListaUsuariosSimulacion $listaTodosUsuarios
   */
  private function setListaTodosUsuarios(ListaUsuariosSimulacion $listaTodosUsuarios) {
    $this->listaTodosUsuarios = $listaTodosUsuarios;
  }

  /**
   * @return ListaUsuariosSimulacion
   */
  public function getListaTodosUsuariosExceptoActual() {
    if (!isset($this->usuarioActual)) {
      return $this->getListaTodosUsuarios();
    } else {
      $listaUsuariosExceptoActual = new ListaUsuariosSimulacion();

      foreach ($this->getListaTodosUsuarios() as $usuario) {
        if ($usuario->getUid() != $this->getUsuarioActual()->getUid()) {
          $listaUsuariosExceptoActual->add($usuario);
        }
      }

      return $listaUsuariosExceptoActual;
    }
  }
}