<?php
namespace Drupal\rjsimulador\ListUtils;

use Exception;
use InvalidArgumentException;
use Drupal\rjsimulador\Filters\FilterInterface;
use Drupal\rjsimulador\DatoInstantaneo;

class ListaDatosInstantaneos extends Lista {
  /**
   * @return DatoInstantaneo
   */
  public function current() {
    return parent::current();
  }

  /**
   * @param int $numberKey Clave del elemento a recuperar.
   * @return DatoInstantaneo Devuelve el item de la lista en esa posición.
   * @throws InvalidArgumentException Si la key pasada no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function get($numberKey) {
    return parent::get($numberKey);
  }

  /**
   * @param DatoInstantaneo $item Elemento a introducir en la lista.
   * @return int Número de elementos en la lista después de añadir el dato.
   * @throws InvalidArgumentException Si el item pasado no es de tipo DatoInstantaneo.
   */
  public function add($item) {
    if ($item instanceof DatoInstantaneo) {
      parent::add($item);
    }
    else {
      throw new InvalidArgumentException("El item a añadir no es de tipo DatoInstantaneo.");
    }

    return $this->count();
  }

  /**
   * @param int $numberKey Clave del elemento al eliminar.
   * @return int Número de elementos en la lista después de eliminar el dato.
   * @throws InvalidArgumentException Si la clave no es numérica.
   * @throws Exception Si no existe esa clave en la lista.
   */
  public function remove($numberKey) {
    return parent::remove($numberKey);
  }

  /**
   * @param FilterInterface $filtro
   * @return ListaDatosInstantaneos Lista de los datos que cumplen con el filtro.
   */
  public function filterBy(FilterInterface $filtro) {
    $listaResultado = new ListaDatosInstantaneos();
    return parent::filterItems($listaResultado, $filtro);
  }
} 