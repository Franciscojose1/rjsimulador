<?php

interface FilterInterface
{
  /**
   * @param mixed $item
   * @return bool Devuelve si el item cumple con los criterios del filtro.
   * @throws Exception Si el elemento es de un tipo no soportado por el filtro.
   */
  public function filter($item);
} 