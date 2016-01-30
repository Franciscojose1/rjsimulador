<?php

class GestorSimulaciones
{
  const SIM_UNO = 1;
  const SIM_DOS = 2;
  const SIM_TRES = 3;
  const SIM_CUATRO = 4;
  const SIM_CINCO = 5;

  /* @var array */
  private $idsSimulacionesCargadas;
  /* @var stdClass $usuarioActual */
  private $usuarioActual;
  /* @var ListaSimulaciones[] Un array de la form $uid => ListaSimulaciones */
  private $arrayListaSimulacionesByUser;

  public function __construct(array $idsSimulacionesACargar, stdClass $usuarioActual)
  {
    $usuarios = entity_load('user');

    foreach ($usuarios as $usuario) {
      $this->arrayListaSimulacionesByUser[$usuario->uid] = new ListaSimulaciones();
      foreach ($idsSimulacionesACargar as $id) {
        $this->arrayListaSimulacionesByUser[$usuario->uid]->add(new Simulacion($id, $usuario));
      }
    }

    $this->setUsuarioActual($usuarioActual);
  }

  /**
   * @return array
   */
  public function getIdsSimulacionesCargadas()
  {
    return $this->idsSimulacionesCargadas;
  }

  /**
   * @param array $idsSimulacionesCargadas
   */
  public function setIdsSimulacionesCargadas(array $idsSimulacionesCargadas)
  {
    $this->idsSimulacionesCargadas = $idsSimulacionesCargadas;
  }

  /**
   * @return stdClass
   */
  public function getUsuarioActual()
  {
    return $this->usuarioActual;
  }

  /**
   * @param stdClass $usuarioActual
   */
  public function setUsuarioActual(stdClass $usuarioActual)
  {
    $this->usuarioActual = $usuarioActual;
  }

  /**
   * @return ListaSimulaciones[]
   */
  public function getArrayListaSimulacionesByUser()
  {
    return $this->arrayListaSimulacionesByUser;
  }

  /**
   * @param stdClass $usuario
   * @return ListaSimulaciones La lista de las simulaciones cargadas para el usuario pasado.
   * @throws LogicException Si no existe el usuario.
   */
  public function getListaSimulacionesByUser(stdClass $usuario) {
    if (array_key_exists($usuario->uid, $this->getArrayListaSimulacionesByUser())) {
      return $this->getArrayListaSimulacionesByUser()[$usuario->uid];
    } else {
      throw new LogicException("No se han cargado las partidas de este usuario o no existe.");
    }
  }

  /**
   * @return ListaSimulaciones La lista de las simulaciones cargadas para el usuario actual de la sesión.
   * @throws LogicException Si no existe el usuario.
   */
  public function getListaSimulacionesUsuarioActual() {
    return $this->getListaSimulacionesByUser($this->getUsuarioActual());
  }

  /**
   * @return ListaPartidas Listado de todas las partidas de todos los usuarios.
   */
  public function retrieveAllPartidas()
  {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getArrayListaSimulacionesByUser() as $listaSimulaciones) {
      foreach ($listaSimulaciones as $simulacion) {
        $listaPartidas->mergeList($simulacion->getListaPartidas());
      }
    }

    return $listaPartidas;
  }

  /**
   * @param int $idSimulation El id de la simulación para la que recuperar las partidas de los usuarios.
   * @return ListaPartidas Lista de todas las partidas de esa simulación de todos los usuarios.
   */
  public function retrieveAllPartidasByIdSimulacion($idSimulation)
  {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getArrayListaSimulacionesByUser() as $listaSimulaciones) {
      foreach ($listaSimulaciones as $simulacion) {
        if (in_array($idSimulation, $this->getIdsSimulacionesCargadas()) && $simulacion->getIdSimulacion() == $idSimulation) {
          $listaPartidas->mergeList($simulacion->getListaPartidas());
        }
      }
    }

    return $listaPartidas;
  }

  /**
   * @param stdClass $usuario
   * @return ListaPartidas Lista de todas las partidas de ese usuario de todas las simulaciones cargadas en el gestor.
   */
  public function retrieveAllPartidasByUser(stdClass $usuario)
  {
    $listaPartidas = new ListaPartidas();

    if (array_key_exists($usuario->uid, $this->getArrayListaSimulacionesByUser())) {
      foreach ($this->getArrayListaSimulacionesByUser()[$usuario->uid] as $simulacion) {
        $listaPartidas->mergeList($simulacion->getListaPartidas());
      }
    }

    return $listaPartidas;
  }

  /**
   * @return ListaPartidas Lista de todas las partidas del usuario actual de todas las simulaciones cargadas en el gestor.
   */
  public function retrieveAllPartidasUsuarioActual() {
    return $this->retrieveAllPartidasByUser($this->getUsuarioActual());
  }
} 