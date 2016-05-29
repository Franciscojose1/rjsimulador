<?php
namespace Drupal\rjsimulador\Filters;

use DateTime;
use Exception, InvalidArgumentException;
use Drupal\rjsimulador\Partida;

/**
 * Class FilterByFecha Permite filtrar por una fecha. Puede filtrar por una fecha de inicio, de fin o ambas a la vez
 * (un intervalo de fechas)
 */
class FilterByFecha implements FilterInterface {
  /* **************************************** */
  /*               CONSTANTES                 */
  /* **************************************** */
  const FECHA_INICIO = 'fecha_inicio';
  const FECHA_FIN = 'fecha_fin';

  /* @var DateTime $fechaInicio */
  private $fechaInicio;
  /* @var DateTime $fechaFin */
  private $fechaFin;
  /* @var bool $includeFechaFin */
  private $includeFechaFin;

  public function __construct(array $arrayFechas, $includeFechaFin = false) {
    if (!isset($arrayFechas[self::FECHA_INICIO]) && !isset($arrayFechas[self::FECHA_FIN])) {
      throw new InvalidArgumentException(t("It is necessary to pass in array FECHA_INICIO or FECHA_FIN at least."));
    }

    if (!is_bool($includeFechaFin)) {
      throw new InvalidArgumentException(t("Variable includeFechaFin must be a boolean."));
    }

    if (isset($arrayFechas[self::FECHA_INICIO])) {
      if ($arrayFechas[self::FECHA_INICIO] instanceof DateTime) {
        $this->fechaInicio = $arrayFechas[self::FECHA_INICIO];
      }
      else {
        throw new InvalidArgumentException("La fecha de inicio debe ser de tipo DateTime");
      }
    }

    if (isset($arrayFechas[self::FECHA_FIN])) {
      if ($arrayFechas[self::FECHA_FIN] instanceof DateTime) {
        $this->fechaFin = $arrayFechas[self::FECHA_FIN];
      }
      else {
        throw new InvalidArgumentException("La fecha de fin debe ser de tipo DateTime");
      }
    }

    $this->includeFechaFin = $includeFechaFin;
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
      if ($this->includeFechaFin) {
        $cumpleFechaMenorQueFin = ($item->getFecha() <= $this->fechaFin->getTimestamp());
      } else {
        $cumpleFechaMenorQueFin = ($item->getFecha() < $this->fechaFin->getTimestamp());
      }
    }

    if (isset($cumpleFechaMayorQueInicio) && isset($cumpleFechaMenorQueFin)) {
      return $cumpleFechaMayorQueInicio && $cumpleFechaMenorQueFin;
    }
    elseif (isset($cumpleFechaMayorQueInicio)) {
      return $cumpleFechaMayorQueInicio;
    }
    elseif (isset($cumpleFechaMenorQueFin)) {
      return $cumpleFechaMenorQueFin;
    }
    else {
      throw new Exception("No existe ninguna fecha con la que comparar el elemento.");
    }
  }
} 