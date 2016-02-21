<?php

class ListaUsuariosSimulacion extends  Lista {
  /**
   * @return UsuarioSimulacion
   */
  public function current() {
    return parent::current();
  }

  /**
   * @param int $numberKey Clave del elemento a recuperar
   * @return UsuarioSimulacion Devuelve el item de la lista en esa posición
   * @throws InvalidArgumentException Si la key pasada no es numérica
   * @throws Exception Si no existe esa clave en la lista
   */
  public function get($numberKey) {
    return parent::get($numberKey);
  }

  /**
   * @param UsuarioSimulacion $item Elemento a introducir en la lista
   * @return int Número de elementos en la lista después de añadir el usuario
   * @throws InvalidArgumentException Si el item pasado no es de tipo UsuarioSimulacion
   */
  public function add($item) {
    if ($item instanceof UsuarioSimulacion) {
      parent::add($item);
    }
    else {
      throw new InvalidArgumentException("El item a añadir no es de tipo UsuarioSimulacion.");
    }

    return $this->count();
  }

  /**
   * @param int $numberKey Clave del elemento al eliminar
   * @return int Número de elementos en la lista después de eliminar el usuario.
   * @throws InvalidArgumentException Si la clave no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function remove($numberKey) {
    return parent::remove($numberKey);
  }

  /**
   * @param ListaUsuariosSimulacion $lista Lista a mezclar con la actual
   */
  public function mergeList(Lista $lista) {
    if ($lista instanceof ListaUsuariosSimulacion) {
      parent::mergeList($lista);
    }
    else {
      throw new InvalidArgumentException("La lista pasada tiene ques ser de tipo ListaUsuariosSimulacion");
    }
  }

  /**
   * @param FilterInterface $filtro
   * @return ListaUsuariosSimulacion Lista de usuarios que cumple con el filtro.
   */
  public function filterBy(FilterInterface $filtro) {
    $listaResultado = new ListaUsuariosSimulacion();
    return parent::filterItems($listaResultado, $filtro);
  }

  public function sortBy($sortField, $sort) {
    $upperOrder = strtoupper($sort);
    if ($upperOrder != "ASC" && $upperOrder != "DESC") {
      throw new Exception("El orden de un campo solo puede tener los valores ASC o DESC");
    }

    if ($sortField != "Uid" && $sortField != "Created" && $sortField != "Login" && $sortField != "LastAccess") {
      throw new Exception("Los únicos campos ordenables son Uid, Created, Login y LastAccess.");
    }

    // Opciones para ordenar la lista. Indica que callable function tiene que ser usado para ordenar los elementos
    // de la lista
    $options = array($this, "sortBy" . $sortField . $upperOrder);
    parent::sortList($options);
  }

  protected static function sortByUidASC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getUid() == $b->getUid()) {
      return 0;
    }

    return ($a->getUid() > $b->getUid()) ? +1 : -1;
  }

  protected static function sortByUidDESC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getUid() == $b->getUid()) {
      return 0;
    }

    return ($a->getUid() < $b->getUid()) ? +1 : -1;
  }

  protected static function sortByNameASC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getName() == $b->getName()) {
      return 0;
    }
    return ($a->getName() < $b->getName()) ? -1 : 1;
  }

  protected static function sortByNameDESC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getName() == $b->getName()) {
      return 0;
    }
    return ($a->getName() > $b->getName()) ? -1 : 1;
  }

  protected static function sortByCreatedASC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getCreationDate() == $b->getCreationDate()) {
      return 0;
    }
    return ($a->getCreationDate() < $b->getCreationDate()) ? -1 : 1;
  }

  protected static function sortByCreatedDESC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getCreationDate() == $b->getCreationDate()) {
      return 0;
    }
    return ($a->getCreationDate() > $b->getCreationDate()) ? -1 : 1;
  }

  protected static function sortByLoginASC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getLoginDate() == $b->getLoginDate()) {
      return 0;
    }
    return ($a->getLoginDate() < $b->getLoginDate()) ? -1 : 1;
  }

  protected static function sortByLoginDESC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getLoginDate() == $b->getLoginDate()) {
      return 0;
    }
    return ($a->getLoginDate() > $b->getLoginDate()) ? -1 : 1;
  }

  protected static function sortByLastAccessASC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getLastAccessDate() == $b->getLastAccessDate()) {
      return 0;
    }
    return ($a->getLastAccessDate() < $b->getLastAccessDate()) ? -1 : 1;
  }

  protected static function sortByLastAccessDESC(UsuarioSimulacion $a, UsuarioSimulacion $b) {
    if ($a->getLastAccessDate() == $b->getLastAccessDate()) {
      return 0;
    }
    return ($a->getLastAccessDate() > $b->getLastAccessDate()) ? -1 : 1;
  }
}