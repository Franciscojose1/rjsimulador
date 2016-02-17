<?php

class FilterByFecha implements FilterInterface {
  /* @var DateTime */
  private $fechaInicio;
  /* @var DateTime */
  private $fechaFin;

  public function __construct(array $arrayFechas) {
    if (isset($arrayFechas['fecha_inicio'])) {
      if ($arrayFechas['fecha_inicio'] instanceof DateTime) {
        $this->fechaInicio = $arrayFechas['fecha_inicio'];
      }
      else {
        throw new InvalidArgumentException("La fecha de inicio debe ser de tipo DateTime");
      }
    }

    if (isset($arrayFechas['fecha_fin'])) {
      if ($arrayFechas['fecha_fin'] instanceof DateTime) {
        $this->fechaFin = $arrayFechas['fecha_fin'];
      }
      else {
        throw new InvalidArgumentException("La fecha de fin debe ser de tipo DateTime");
      }
    }
  }

  /**
   * @param mixed $item
   * @return bool Devuelve si el item cumple con los criterios de fecha.
   * @throws Exception Si el elemento es de un tipo no soportado por el filtro.
   */
  public function filter($item) {
    if ($item instanceof Partida) {
      return $this->filterPartidaByFecha($item);
    }
    else {
      throw new Exception("El item pasado no es un tipo soportado.");
    }
  }

  /**
   * @param Partida $item
   * @return bool Devuelve si la Partida cumple con los criterios de fecha.
   * @throws Exception
   */
  private function filterPartidaByFecha(Partida $item) {
    if (isset($this->fechaInicio)) {
      $cumpleFechaMayorQueInicio = ($item->getFecha() >= $this->fechaInicio->getTimestamp());
    }

    if (isset($this->fechaFin)) {
      $cumpleFechaMenorQueFin = ($item->getFecha() < $this->fechaFin->getTimestamp());
    }

    if (isset($cumpleFechaMayorQueInicio) && isset($cumpleFechaMenorQueFin)) {
      return $cumpleFechaMayorQueInicio && $cumpleFechaMenorQueFin;
    }
    else {
      if (isset($cumpleFechaMayorQueInicio)) {
        return $cumpleFechaMayorQueInicio;
      }
      else {
        if (isset($cumpleFechaMenorQueFin)) {
          return $cumpleFechaMenorQueFin;
        }
        else {
          throw new Exception("No existe ninguna fecha con la que comparar el elemento.");
        }
      }
    }
  }

} 