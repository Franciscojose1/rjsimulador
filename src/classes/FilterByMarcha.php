<?php

class FilterByMarcha implements FilterInterface {
  /* @var array */
  private $marchas;

  public function __construct($paramMarchas) {
    foreach ($paramMarchas as $marcha) {
      if (!is_integer($marcha)) {
        throw new InvalidArgumentException("Los IDs pasados tienen que ser un número entero.");
      }
    }
    $this->marchas = $paramMarchas;
  }

  /**
   * @param mixed $item
   * @return bool Devuelve si el item cumple con los criterios del filtro.
   * @throws Exception Si el elemento es de un tipo no soportado por el filtro.
   */
  public function filter($item) {
    if ($item instanceof DatoInstantaneo) {
      return $this->filterDatoByMarcha($item);
    }
    else {
      throw new Exception("El item pasado no es un tipo soportado.");
    }
  }

  public function filterDatoByMarcha(DatoInstantaneo $item) {
    foreach ($this->marchas as $marcha) {
      if ($item->getMarcha() == $marcha) {
        return TRUE;
      }
    }

    return FALSE;
  }
}