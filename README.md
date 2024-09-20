# Short Transform

**Оглавление:**
- [Short Transform](#short-transform)
- [Сборка проекта](#сборка-проекта)
- [Формулировка задачи](#формулировка-задачи)
  - [Пример работы программы](#пример-работы-программы)
  - [Мои рассуждения (эвристика)](#мои-рассуждения-эвристика)
- [Алгоритм решения задачи](#алгоритм-решения-задачи)
- [Характеристики алгоритма](#характеристики-алгоритма)
  - [Затраты памяти](#затраты-памяти)
  - [Алгоритмическая сложность](#алгоритмическая-сложность)

# Сборка проекта
Все способы сборки данного проекта описаны в файле `CMakePresets.json`.<br/>
```
Пример команды сборки:
    cmake . --preset Release_Preset && cmake --build --preset Release_Build
```
Если у нас `vscode` или `vscodium`, то дополнительно нужно установить расширение `CMake Tools` и внутри него выбрать нужный пресет. Для того, чтобы в `vscode` или `vscodium` производить debug'инг приложения, нужно добавить в корень проекта файл `.vscode/launch.json` с таким содержимым:
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug with CMake",
            "type": "lldb", // Here your debugger
            "request": "launch",
            "program": "${command:cmake.launchTargetPath}"
        }
    ]
}
```
После этого у вас появится возможность просто нажимая `F5` производить отладку в привычном формате.

# Формулировка задачи
```
Даны две строки s и f (начальная и конечная) и словарь D (набор слов).

Нужно определить, можно ли преобразовать s в f, используя только слова из словаря D. При этом каждое преобразование должно менять только один символ, а длина слова должна оставаться неизменной. Если преобразование возможно, нужно найти кратчайшую последовательность таких преобразований и вернуть ее длину. Если преобразование невозможно, вернуть "Преобразование невозможно".
```

## Пример работы программы
```
Входные данные:
    D = { cat, cot, dot, dog, bat, dag }
    s = cat
    f = dog

Вывод:
    Минимальное количество шагов для преобразования 'cat' в 'dog': 3
```
```
Входные данные:
    D = { cat, cot, bat }
    s = cat
    f = dog

Вывод:
    Преобразование невозможно
```

## Мои рассуждения (эвристика)
Давайте создадим 'дерево трансформаций', то есть дерево, где каждым узлом будет слово, а если два узла будут соединены между собой, то это означает, что от одного слова до другого можно 'добраться' соблюдая правила описанные в главе [Формулировка задачи](#формулировка-задачи).

В таком случае, за начало дерева возьмем стартовое слово.

Рассмотрим на примере:
```
Входные данные:
    D = { cat, cot, dot, dog, bat, dag, hat }
    s = cat
    f = dog
```

В самом начале наше дерево трансформаций будет выглядеть следующим образом:
```
<cat>
```
Пока что у нас есть всего один узел равный стартовому слову.
> Если стартового (или конечного) слова нет в словаре преобразований, то мы можем сразу сказать `(Преобразование невозможно)`, т.к. по условию задачи мы можем делать преобразования только строго следуя словарю преобразований.

> Если длина начального слова != длине конечного слова, то мы тоже сразу же можем сказать `(Преобразование невозможно)`, т.к. для того, чтобы произвести такое преобразование, нужно как минимум 1 раз изменить длину слова, что по условию задачи делать запрещено.

> Если длина конкретного слова из словаря преобразований != длине начальному слову, то это конкретное слово можно удалить из словаря, т.к. до него невозможно добраться не изменив длину, что по условию задачи делать запрещено.

Дальше нам нужно понять, какое слово нам дальше следует поставить в наше дерево преобразований.

От нас требуется либо найти кротчайшию длину трансформаций начального слова в конечный, либо сказать, что такого пути не существует.

```
Рассмотрим такой случай:
    D = { cat, bat, hat, ... }
    s = cat
```

Для такого словаря и такого стартового слова существуют как минимум 3 возможных дерева трансформаций:
```
№1

      <cat>
     /  |  \
<...> <bat> <...>
     /  |  \
<...> <hat> <...>
     /  |  \
<...> <...> <...>
```
```
№1.1

      <cat>
     /  |  \
<...> <hat> <...>   <-\
     /  |  \           -- Слова 'hat' и 'bat' поменялись местами
<...> <bat> <...>   <-/
     /  |  \
<...> <...> <...>
```
```
№2

      <cat> ______
     /  |  \      \
<...> <bat> <hat> <...>
        |     |
      <...> <...>
```
Действительно, в случае №1 мы можем в слове 'cat' заменить только лишь первую букву 'c' -> 'b' и получить новое слово 'bat', а потом в слове 'bat' заменить первую букву 'b' -> 'h' чтобы получить новое слово 'hat'.

В случае №1.1 всё тоже самое, только слова 'bat' и 'hat' поменяны местами.

В случае №2 мы можем 'добраться' до слов 'bat' и 'hat' сразу из стартового слова заменой первых букв на соответствующие для производных слов.

Какое же дерево трансформаций более предпочтительнее для решения поставленной задачи? Для этого нужно вспомнить, что от нас требуется: `...нужно найти кратчайшую длину последовательности...`. Тогда, давайте выберем то дерево трансформаций, у которого для **ВСЕХ** его слов будет кратчайшая длина трансформации от стартового слова. ~~Благо у нас нет ограничений, препятствующих данному выбору~~. Действительно, если у **КАЖДОГО** слова в дереве трансформации будет минимальная длина трансформации, то она будет минимальная и у конечного слова, что от нас и требуется.

Посмотрим, какое из этих 3-х деревьев нам подходит больше всего.
```
№1

|Количество трансформаций: 0|       <cat>
                                   /  |  \
|Количество трансформаций: 1| <...> <bat> <...>
                                   /  |  \
|Количество трансформаций: 2| <...> <hat> <...>
                                   /  |  \
|Количество трансформаций: 3| <...> <...> <...>
```
```
№2

|Количество трансформаций: 0|       <cat> ______
                                   /  |  \      \
|Количество трансформаций: 1| <...> <bat> <hat> <...>
                                      |     |
|Количество трансформаций: 2|       <...> <...>
```
~~Рассуждения для дерева №1.1 будут индетичны с деревом №1~~.<br/>
Как видно из рисунков, единственное, чем отличаются данные деревья, так это веткой со словом 'hat'. В дереве №2 слово находится на уровне выше по сравнению с деревом №1 => количество трансформаций меньше на 1, а это хорошо. **НО**, если в дереве №1 были какие-то другие слова, которые можно было получить от слова 'hat', то в дереве №2 для **ВСЕХ** них количество трансформации тоже уменьшилось на 1, а если в этой ветке есть наше конечное слово, то нам это только на руку, ведь это от нас и требуется.

Из этих рассуждений следует, что нам следует выбрать дерево трансформаций №2.
> Если от текущего слова существует *N* слов, до которых можно 'добраться' по правилам данной задачи, то можно смело добавлять **ВСЕ** эти слова как производные от текущего слова.

```
У нас ещё может быть и такой случай:
    D = { cat, hat, hat, ... }
    s = cat
```
Дерево трансформации для него будет выглядеть следующим образом:
```
      <cat> ______
     /  |  \      \
<...> <hat> <hat> <...>
        | \ / |
      <...> <...>
```
В таком дереве ветки после слов 'hat' будут индентичными. Действительно, ведь если можно добраться от стартового слова до слова <...> по 'правому' пути, то можно и по левому (т.к. слова 'hat' и 'hat' индентичны). Но от нас требуется не количество возможных преобразований от стартового слова до конечного, а минимально возможная длина преобразований хотя бы одной ветки. Следовательно, дубликаты слов из словаря преобразований можно удалить.

> Если в данном словаре преобразований есть дубликаты, то их можно смело удалять. Это сэкономит нам память и время.

# Алгоритм решения задачи
На самом деле, нам не нужно в памяти хранить именно 'дерево'. Нам не нужна информация о путях преобразований. Нужна лишь информация об длине преобразований и о том, есть ли хоть один путь преобразований от начального слова до конечного. Следовательно, всё можно упростить до обычного массива, элементами которого будут {слово | можно ли сделать преобразование до текущего слова или нет | минимальная длина преобразований}.

**Начальное состояние массива:**
| index | word  | isUsed | depthDegree |
| :---: | :---: | :----: | :---------: |
|   0   |  cat  |   -    |      0      |
|   1   |  cot  |   -    |      0      |
|   2   |  dot  |   -    |      0      |
|   3   |  dog  |   -    |      0      |
|   4   |  bat  |   -    |      0      |
|   5   |  dag  |   -    |      0      |
|   6   |  hat  |   -    |      0      |

Для начального слова ставим *isUsed* = true и depthDegree = 1. Это эквивалентно добавлению вершины в дерево преобразований.

**Массив готов к запуску алгоритма**
| index | word  | isUsed | depthDegree |
| :---: | :---: | :----: | :---------: |
|   0   |  cat  |   +    |      1      |
|   1   |  cot  |   -    |      0      |
|   2   |  dot  |   -    |      0      |
|   3   |  dog  |   -    |      0      |
|   4   |  bat  |   -    |      0      |
|   5   |  dag  |   -    |      0      |
|   6   |  hat  |   -    |      0      |

Далее для всех слов, у кого depthDegree равен текущей длине преобразований (curDepthDegree) (в начале это 1) мы ищем другие слова, которые можно преобразовать от текущих соблюдая правила задачи. Для всех таких слов мы говорим {isUsed = true, depthDegree = curDepthDegree + 1}. После того, как мы прошлись по всем словам мы увеличиваем curDepthDegree на 1 и повторяем алгоритм до тек пор, пока мы смогли сделать хотя бы одно преобразование за цикл.

**Массив после выполнения алгоритма**
| index | word  | isUsed | depthDegree |
| :---: | :---: | :----: | :---------: |
|   0   |  cat  |   +    |      1      |
|   1   |  cot  |   +    |      2      |
|   2   |  dot  |   +    |      3      |
|   3   |  dog  |   +    |      4      |
|   4   |  bat  |   +    |      2      |
|   5   |  dag  |   +    |      5      |
|   6   |  hat  |   +    |      2      |

После этого мы смотрим на конечное слово в данном масиве. Если его isUsed == false, то это означает, что не существует ни один способ преобразовать начальное слово в конечное. Если его isUsed == true, то это значит что существует хотя бы один способ преобразовать начальное слово в конечное, тогда (depthDegree - 1) это будет минимальная длина преобразований от начального слова до конечного.
> Мы вычитаем единицу, т.к. у начального слова по умолчанию стоит единица ~~подумайте сами, почему нельзя оставлять 0 для начального слова в данном алгоритме~~.

```
Вывод:
    Минимальное количество шагов для преобразования 'cat' в 'dog': 3
```

# Характеристики алгоритма
## Затраты памяти
Затраты памяти можно расчитать по формуле:<br/>
$M \propto N$<br/>
$M = N*(l+1) + 2I$, где
- $M$ - количество байт на весь массив.
- $N$ - количество слов в словаре преобразований после его 'чистки'.
- $l$ - длина каждого слова в байтах
- $I$ - количество байт, отводящиеся на `index` и `depthDegree`.

## Алгоритмическая сложность
Алгоритмическая сложность $= O(n^3)$.