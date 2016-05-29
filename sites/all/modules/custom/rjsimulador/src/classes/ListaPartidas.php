<?php
namespace RJSimulador\ListUtils;

use Exception, InvalidArgumentException;
use RJSimulador\Filters\FilterInterface;
use RJSimulador\Partida;

class ListaPartidas extends Lista implements SortableInterface {
  /**
   * @return Partida
   */
  public function current() {
    return parent::current();
  }

  /**
   * @param int $numberKey Clave del elemento a recuperar.
   * @return Partida Devuelve el item de la lista en esa posición.
   * @throws InvalidArgumentException Si la key pasada no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function get($numberKey) {
    return parent::get($numberKey);
  }

  /**
   * @param Partida $item Elemento a introducir en la lista.
   * @return int Número de elementos en la lista después de añadir la partida.
   * @throws InvalidArgumentException Si el item pasado no es de tipo Partida.
   */
  public function add($item) {
    if ($item instanceof Partida) {
      parent::add($item);
    }
    else {
      throw new InvalidArgumentException("El item a añadir no es de tipo Partida.");
    }

    return $this->count();
  }

  /**
   * @param int $numberKey Clave del elemento al eliminar.
   * @return int Número de elementos en la lista después de eliminar la partida.
   * @throws InvalidArgumentException Si la clave no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function remove($numberKey) {
    return parent::remove($numberKey);
  }

  /**
   * @param Lista $lista Lista a mezclar con la actual.
   */
  public function mergeList(Lista $lista) {
    if ($lista instanceof ListaPartidas) {
      parent::mergeList($lista);
    }
    else {
      throw new InvalidArgumentException("La lista pasada tiene ques ser de tipo ListaPartida");
    }
  }

  /**
   * @param FilterInterface $filtro
   * @return ListaPartidas Lista de partidas que cumple con el filtro.
   */
  public function filterBy(FilterInterface $filtro) {
    $listaResultado = new ListaPartidas();
    return parent::filterItems($listaResultado, $filtro);
  }

  /**
   * @inheritdoc
   */
  public function sortBy($sortField, $sort) {
    $upperOrder = strtoupper($sort);
    if ($upperOrder != "ASC" && $upperOrder != "DESC") {
      throw new Exception("El orden de un campo solo puede tener los valores ASC o DESC");
    }

    if ($sortField != "Fecha" && $sortField != 'NombreSimulacion') {
      throw new Exception("Los únicos campos ordenables son Fecha y NombreSimulacion.");
    }

    // Opciones para ordenar la lista. Indica que callable function tiene que ser usado para ordenar los elementos
    // de la lista
    $options = array($this, "sortBy" . $sortField . $upperOrder);
    parent::sortList($options);
  }

  protected static function sortByFechaASC(Partida $a, Partida $b) {
    if ($a->getFecha() == $b->getFecha()) {
      return 0;
    }

    return ($a->getFecha() > $b->getFecha()) ? +1 : -1;
  }

  protected static function sortByFechaDESC(Partida $a, Partida $b) {
    if ($a->getFecha() == $b->getFecha()) {
      return 0;
    }

    return ($a->getFecha() < $b->getFecha()) ? +1 : -1;
  }

  protected static function sortByNombreSimulacionASC(Partida $a, Partida $b) {
    $aNombreSimulacion = strtolower($a->getNombreSimulacion());
    $bNombreSimulacion = strtolower($b->getNombreSimulacion());
    if ($aNombreSimulacion == $bNombreSimulacion) {
      return 0;
    }
    return ($aNombreSimulacion > $bNombreSimulacion) ? +1 : -1;
  }

  protected static function sortByNombreSimulacionDESC(Partida $a, Partida $b) {
    $aNombreSimulacion = strtolower($a->getNombreSimulacion());
    $bNombreSimulacion = strtolower($b->getNombreSimulacion());
    if ($aNombreSimulacion == $bNombreSimulacion) {
      return 0;
    }
    return ($aNombreSimulacion < $bNombreSimulacion) ? +1 : -1;
  }
}