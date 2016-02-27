<?php

class FilterByInterval implements FilterInterface {
  const AGE = 0;
  const DRIVING_EXPERIENCE = 1;
  const AVERAGE_ANNUAL_MILEAGE = 2;

  private $field;

  private $desde;

  private $hasta;

  public function __construct(array $paramInterval, $paramField) {
    if (!isset($paramInterval['desde']) || !isset($paramInterval['hasta']) || !isset($paramField)) {
      throw new InvalidArgumentException("Son necesarios los parámetros desde, hasta y el tipo de intervalo.");
    }

    $this->field = $paramField;
    $this->desde = $paramInterval['desde'];
    $this->hasta = $paramInterval['hasta'];
  }

  public function filter($item) {
    if ($item instanceof UsuarioSimulacion) {
      switch ($this->field) {
        case self::AGE:
          return $this->filterUserByAge($item);
          break;
        case self::DRIVING_EXPERIENCE:
          return $this->filterUserByDrivingExperience($item);
          break;
        case self::AVERAGE_ANNUAL_MILEAGE:
          return $this->filterUserByAverageAnnualMileage($item);
          break;
        default:
          throw new Exception("No se puede procesar el campo pasado para el tipo UsuarioSimulacion.");
          break;
      }
    }
    else {
      throw new Exception("El item pasado no es un tipo soportado.");
    }
  }

  private function filterUserByAge(UsuarioSimulacion $item) {
    // Comprobamos el usuario tenga una edad entre los valores proporcionados
    return $item->getAge() >= $this->desde && $item->getAge() < $this->hasta;
  }

  private function filterUserByDrivingExperience(UsuarioSimulacion $item) {
    // Comprobamos el usuario tenga una experiencia de conducción entre los valores
    return $item->getDrivingExperience() >= $this->desde && $item->getDrivingExperience() < $this->hasta;
  }

  private function filterUserByAverageAnnualMileage(UsuarioSimulacion $item) {
    // Comprobamos el usuario tenga una experiencia de conducción entre los valores
    return $item->getAverageAnnualMileage() >= $this->desde && $item->getAverageAnnualMileage() < $this->hasta;
  }
}