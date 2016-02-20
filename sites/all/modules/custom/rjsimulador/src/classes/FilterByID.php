<?php

class FilterByID implements FilterInterface {
  /* @var array $ids */
  private $ids;

  public function __construct(array $paramIDs) {
    foreach ($paramIDs as $id) {
      if (!is_integer($id)) {
        throw new InvalidArgumentException("Los IDs pasados tienen que ser un número entero.");
      }
    }
    $this->ids = $paramIDs;
  }

  /**
   * @param mixed $item
   * @return bool Devuelve si el item el id pertenece al array de ids pasados.
   * @throws Exception Si el elemento es de un tipo no soportado por el filtro.
   */
  public function filter($item) {
    if ($item instanceof Infraccion) {
      return $this->filterInfraccionByID($item);
    }
    else {
      throw new Exception("El item pasado no es un tipo soportado.");
    }
  }

  private function filterInfraccionByID(Infraccion $item) {
    foreach ($this->ids as $id) {
      if ($id == $item->getIdInfraccion()) {
        return TRUE;
      }
    }

    return FALSE;
  }
}