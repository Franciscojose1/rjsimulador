<?php

abstract class Lista implements Iterator, Countable
{
  private $count = 0;
  private $position = 0;
  private $items = array();

  public function __construct()
  {
    $this->count = 0;
    $this->position = 0;
  }

  /**
   * Return the current element
   * @return mixed Can return any type.
   */
  public function current()
  {
    return $this->items[$this->position];
  }

  /**
   * Move forward to next element
   * @return void Any returned value is ignored.
   */
  public function next()
  {
    ++$this->position;
  }

  /**
   * Return the key of the current element
   * @return mixed scalar on success, or null on failure.
   */
  public function key()
  {
    return $this->position;
  }

  /**
   * Checks if current position is valid
   * @return boolean The return value will be casted to boolean and then evaluated.
   * Returns true on success or false on failure.
   */
  public function valid()
  {
    return isset($this->items[$this->position]);
  }

  /**
   * Rewind the Iterator to the first element
   * @return void Any returned value is ignored.
   */
  public function rewind()
  {
    $this->position = 0;
  }

  /**
   * Count elements of an object
   * @return int The custom count as an integer.
   * The return value is cast to an integer.
   */
  public function count()
  {
    return $this->count;
  }

  /**
   * @param int $newCount nuevo número de elemento de la lista
   */
  private function setCount($newCount)
  {
    $this->count = $newCount;
  }

  /**
   * @param int $numberKey Clave del elemento a recuperar
   * @return mixed Devuelve el item de la lista en esa posición
   * @throws InvalidArgumentException Si la key pasada no es numérica
   * @throws Exception Si no existe esa clave en la lista
   */
  public function get($numberKey)
  {
    if (!is_numeric($numberKey)) {
      throw new InvalidArgumentException("La clave para recuperar un elemento debe ser numérica.");
    }

    if ($numberKey >= $this->count()) {
      throw new Exception("No existe un elemento con esa clave en la lista.");
    }

    return $this->items[$numberKey];
  }

  /**
   * @param mixed $item Item a añadir a la lista
   * @return int Número de elementos en la lista después de añadir el item.
   */
  public function add($item)
  {
    $this->items[$this->count()] = $item;
    $this->setCount($this->count() + 1);
    return $this->count();
  }

  /**
   * @param int $numberKey Clave del elemento al eliminar
   * @return int Número de elementos en la lista después de eliminar el item.
   * @throws InvalidArgumentException Si la clave no es numérica
   * @throws Exception Si no existe esa clave en la lista
   */
  public function remove($numberKey)
  {
    if (!is_numeric($numberKey)) {
      throw new InvalidArgumentException("La clave para eliminar un elemento debe ser numérica.");
    }

    if ($numberKey < $this->count()) {
      unset($this->items[$numberKey]);
      ksort($this->items);
      $this->setCount($this->count() - 1);
    } else {
      throw new Exception("No existe un elemento con esa clave en la lista.");
    }

    return $this->count();
  }

  /**
   * @param Lista $lista Lista a fusionar con la actual
   */
  public function mergeList(Lista $lista)
  {
    $this->items = array_merge($this->items, $lista->items);
    ksort($this->items);
    $this->setCount($this->count() + $lista->count());
  }

  /**
   * @param mixed $options Opciones para ordenar la lista.
   */
  protected function sortList($options)
  {
    usort($this->items, $options);
  }

  protected function filterItems(Lista $listaResultado, FilterInterface $filter)
  {
    foreach ($this->items as $item) {
      if ($filter->filter($item)) {
        $listaResultado->add($item);
      }
    }

    return $listaResultado;
  }
} 