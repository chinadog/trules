# TRules

## Описание

Библиотека разбора событий.


## Установка

### Шаг 1: Установка префикса
Выполните qmake, при необходимости установив в PREFIX путь, в который
необходимо произвести установку компоненты. По умолчанию префикс будет равен
текущему рабочему каталогу. Данная команда подготовит make файл для сборки
проекта.
```
qmake
или
qmake PREFIX=~/DST/
```

### Шаг 2: Сборка
Произведите сборку проекта, используя инструкции makefile созданного в
первом шаге. Для это выполните команду make. Данная команда произведет сборку
проекта в исполняемый файл и подготовит его к установке.
```
make
```
### Шаг 3: Установка
Для установки приложения в заданный на первом шаге путь PREFIX, необходимо
выполнить команду make install. Что при успешном выполнении приведет к
установке всех необходимых файлов в заданную директорию.
```
make install
```

## Конфигурация

