<?php
namespace Drupal\rjsimulador\DataCalculation;

use Exception;
use Drupal\rjsimulador\ListUtils\Lista, Drupal\rjsimulador\ListUtils\ListaPartidas, Drupal\rjsimulador\ListUtils\ListaDatosInstantaneos;

/**
 * Class CalculateAverageData Clase que permite calcular datos medios la Lista recibida.
 */
class CalculateAverageData implements CalculatedDataInterface {
  const CONSUMO_MEDIO = 0;
  const TIEMPO_TOTAL = 1;
  const VELOCIDAD = 2;
  const DESVIACION_VELOCIDAD = 3;
  const RPM = 4;
  const DESVIACION_RPM = 5;

  private $field;

  /**
   * @param int $field Debe ser una de las constantes de la clase CalculateAverageData.
   */
  public function __construct($field) {
    $this->field = $field;
  }

  /**
   * @inheritdoc
   */
  public function calculate(Lista $lista) {
    if ($lista instanceof ListaPartidas) {
      switch ($this->field) {
        case self::CONSUMO_MEDIO:
          return $this->calculateMediaDeConsumoMedioListaPartidas($lista);
          break;
        case self::TIEMPO_TOTAL:
          return $this->calculateMediaDeTiempoTotalMedioListaPartidas($lista);
          break;
        case self::VELOCIDAD:
          return $this->calculateMediaDeVelocidadesMediasListaPartidas($lista);
          break;
        case self::DESVIACION_VELOCIDAD:
          return $this->calculateMediaDeDesviacionesTipicasVelocidadListaPartidas($lista);
          break;
        case self::RPM:
          return $this->calculateMediaDeRpmsMediasListaPartidas($lista);
          break;
        case self::DESVIACION_RPM:
          return $this->calculateMediaDeDesviacionesTipicasRpmsListaPartidas($lista);
          break;
        default:
          throw new Exception("No se puede procesar el campo pasado para el tipo ListaPartidas.");
          break;
      }
    }
    elseif ($lista instanceof ListaDatosInstantaneos) {
      switch ($this->field) {
        case self::VELOCIDAD:
          return $this->calculateVelocidadMediaPartida($lista);
          break;
        case self::RPM:
          return $this->calculateRpmsMediasPartida($lista);
          break;
        default:
          throw new Exception("No se puede procesar el campo pasado para el tipo ListaDatosInstantaneos.");
          break;
      }
    }
    else {
      throw new Exception("La lista pasada no es de un tipo soportado");
    }
  }

  /**
   * @param ListaPartidas $lista
   * @return float Consumo medio entre todas las partidas de la lista pasada.
   */
  private function calculateMediaDeConsumoMedioListaPartidas(ListaPartidas $lista) {
    $resultadoConsumoMedio = 0;

    if ($lista->count() > 0) {
      $consumoMedio = 0;

      foreach ($lista as $partida) {
        $consumoMedio += $partida->getConsumoMedio();
      }

      $resultadoConsumoMedio = $consumoMedio / $lista->count();
    }

    return $resultadoConsumoMedio;
  }

  /**
   * @param ListaPartidas $lista
   * @return float Tiempo total entre todas las partidas de la lista pasada.
   */
  private function calculateMediaDeTiempoTotalMedioListaPartidas(ListaPartidas $lista) {
    $resultadoTiempoTotalMedio = 0;

    if ($lista->count() > 0) {
      $tiempoTotalMedio = 0;

      foreach ($lista as $partida) {
        $tiempoTotalMedio += $partida->getTiempoTotal();
      }

      $resultadoTiempoTotalMedio = $tiempoTotalMedio / $lista->count();
    }

    return $resultadoTiempoTotalMedio;
  }

  /**
   * Devuelve la velocidad media de una lista de partidas.
   * @param ListaPartidas $lista
   * @return float|int
   */
  private function calculateMediaDeVelocidadesMediasListaPartidas(ListaPartidas $lista) {
    $resultadoVelocidadMediaTotal = 0;

    if ($lista->count() > 0) {
      $velocidadMediaTotal = 0;

      foreach ($lista as $partida) {
        $velocidadMediaTotal += $partida->getVelocidadMedia();
      }

      $resultadoVelocidadMediaTotal = $velocidadMediaTotal / $lista->count();
    }

    return $resultadoVelocidadMediaTotal;
  }

  private function calculateMediaDeDesviacionesTipicasVelocidadListaPartidas(ListaPartidas $lista) {
    $resultadoMediaDTVelocidad = 0;

    if ($lista->count() > 0) {
      $sumaDesviacionesVelocidad = 0;

      foreach ($lista as $partida) {
        $sumaDesviacionesVelocidad += $partida->getDesviacionTipicaVelocidad();
      }

      $resultadoMediaDTVelocidad = $sumaDesviacionesVelocidad / $lista->count();
    }

    return $resultadoMediaDTVelocidad;
  }

  /**
   * Devuelve las RPMs medias de una lista de partidas.
   * @param ListaPartidas $lista
   * @return float|int
   */
  private function calculateMediaDeRpmsMediasListaPartidas(ListaPartidas $lista) {
    $resultadoRpmsMediasTotal = 0;

    if ($lista->count() > 0) {
      $rpmsMediaTotal = 0;

      foreach ($lista as $partida) {
        $rpmsMediaTotal += $partida->getRpmMedia();
      }

      $resultadoRpmsMediasTotal = $rpmsMediaTotal / $lista->count();
    }

    return $resultadoRpmsMediasTotal;
  }

  private function calculateMediaDeDesviacionesTipicasRpmsListaPartidas(ListaPartidas $lista) {
    $resultadoMediaDesviacionTipicaRpms = 0;

    if ($lista->count() > 0) {
      $sumaDesviacionesRpms = 0;

      foreach ($lista as $partida) {
        $sumaDesviacionesRpms += $partida->getDesviacionTipicaRpm();
      }

      $resultadoMediaDesviacionTipicaRpms = $sumaDesviacionesRpms / $lista->count();
    }

    return $resultadoMediaDesviacionTipicaRpms;
  }

  /**
   * @param ListaDatosInstantaneos $lista
   * @return float Velocidad media de una partida.
   */
  private function calculateVelocidadMediaPartida(ListaDatosInstantaneos $lista) {
    $resultadoVelocidadTotal = 0;

    if ($lista->count() > 0) {
      $velocidadTotal = 0;

      foreach ($lista as $dato) {
        $velocidadTotal += $dato->getVelocidad();
      }

      $resultadoVelocidadTotal = $velocidadTotal / $lista->count();
    }

    return $resultadoVelocidadTotal;
  }

  /**
   * @param ListaDatosInstantaneos $lista
   * @return float RPMs medias de una partida.
   */
  private function calculateRpmsMediasPartida(ListaDatosInstantaneos $lista) {
    $resultadoRpmTotal = 0;

    if ($lista->count() > 0) {
      $velocidadRpm = 0;

      foreach ($lista as $dato) {
        $velocidadRpm += $dato->getRpm();
      }

      $resultadoRpmTotal = $velocidadRpm / $lista->count();
    }

    return $resultadoRpmTotal;
  }
}