<?php
namespace Drupal\rjsimulador\ListUtils;

use Exception;

/**
 * Interface SortableInterface Interfaz que deben implementar las listas ordenables
 */
interface SortableInterface {
  /**
   * Ordena la lista en función del campo pasado en el orden dado.
   * @param string $sortField El campo por el que ordenar.
   * @param string $sort Puede ser ASC o DESC.
   * @throws Exception Si el campo pasado o el tipo de ordenación no es aceptable.
   */
  public function sortBy($sortField, $sort);
} 