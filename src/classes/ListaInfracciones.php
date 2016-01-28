<?php
class ListaInfracciones extends Lista
{
  /**
   * @return Infraccion
   */
  public function current()
  {
    return parent::current();
  }

  /**
   * @param int $numberKey Clave del elemento a recuperar
   * @return Infraccion Devuelve el item de la lista en esa posición
   * @throws InvalidArgumentException Si la key pasada no es numérica
   * @throws Exception Si no existe esa clave en la lista
   */
  public function get($numberKey)
  {
    return parent::get($numberKey);
  }

  /**
   * @param Infraccion $item Elemento a introducir en la lista
   * @return int Número de elementos en la lista después de añadir la infracción
   * @throws InvalidArgumentException Si el item pasado no es de tipo Infracción
   */
  public function add($item)
  {
    if ($item instanceof Infraccion) {
      parent::add($item);
    } else {
      throw new InvalidArgumentException("El item a añadir no es de tipo Infracción.");
    }

    return $this->count();
  }

  /**
   * @param int $numberKey Clave del elemento al eliminar
   * @return int Número de elementos en la lista después de eliminar la infracción.
   * @throws InvalidArgumentException Si la clave no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function remove($numberKey)
  {
    return parent::remove($numberKey);
  }

  /**
   * @param ListaInfracciones $lista Lista a mezclar con la actual.
   */
  public function mergeList(Lista $lista)
  {
    if ($lista instanceof ListaInfracciones) {
      parent::mergeList($lista);
    } else {
      throw new InvalidArgumentException("La lista pasada tiene que ser de tipo ListaInfracciones");
    }
  }

  /**
   * @param FilterInterface $filtro
   * @return ListaInfracciones Lista de partidas que cumple con el filtro.
   */
  public function filterBy(FilterInterface $filtro)
  {
    $listaResultado = new ListaInfracciones();
    return parent::filterItems($listaResultado, $filtro);
  }
} 