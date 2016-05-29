<?php
namespace Drupal\rjsimulador\Controllers;

use Exception;
use LogicException;
use Drupal\rjsimulador\Factory\FactoryDataManager;
use Drupal\rjsimulador\ListUtils\ListaUsuariosSimulacion;
use Drupal\rjsimulador\UsuarioSimulacion;

/**
 * Class GestorSimulaciones Realiza tareas de gestión para un usuario.
 */
class GestorSimulaciones {
  /* @var UsuarioSimulacion $usuarioActual EL usuario actual. */
  private $usuarioActual;
  /* @var ListaUsuariosSimulacion $listaTodosUsuarios Todos los usuarios que tienen partidas almacenadas de las Simulaciones. */
  private $listaTodosUsuarios;
  /* @var array $arrayUsuariosUidName Array de todos los usuarios de la forma $uid=>$name */
  private $arrayUsuariosUidName;

  /**
   * @param UsuarioSimulacion $usuarioActual El usuario actual.
   * @throws LogicException Si no existen usuarios en la BBD.
   */
  public function __construct(UsuarioSimulacion $usuarioActual = NULL) {
    $provider = FactoryDataManager::createDataProvider();

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
    }
    else {
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
   * @return array
   */
  public function getArrayUsuariosUidName() {
    if (!isset($this->arrayUsuariosUidName)) {
      // Recuperamos el array de la lista de todos los usuarios
      foreach ($this->getListaTodosUsuarios() as $usuario) {
        $this->arrayUsuariosUidName[$usuario->getUid()] = $usuario->getName();
      }
    }

    return $this->arrayUsuariosUidName;
  }

  /**
   * @return ListaUsuariosSimulacion
   */
  public function getListaTodosUsuariosExceptoActual() {
    if (!isset($this->usuarioActual)) {
      return $this->getListaTodosUsuarios();
    }
    else {
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