<?php

interface SortableInterface {
  /**
   * Ordena la lista en función del campo pasado en el orden dado.
   * @param string $sortField El campo por el que ordenar.
   * @param string $sort Puede ser ASC o DESC.
   */
  public function sortBy($sortField, $sort);
} 