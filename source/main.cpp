/**
 * @file main.cpp
 * @author LAVRIK596 (Lavrik596@mail.ru)
 * @brief Основной файл для решения данной задачи.
 * @date 2024-09-11
 *
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Структура описания токена (слова).
 *
 */
struct treeNode
{
  std::string word{}; ///< Само слово (токен).
  /**
   * @brief Было ли оно использовано (если да, то это значит, что есть хотя бы 1 путь трансформации от начального слова
   * до текущего).
   */
  bool isUsed{false};
  size_t depthDegree{0}; ///< Степень глубины (кратчайщее расстояние трансформации от начального слова до текущего).
};

using dictionary_t = std::vector<treeNode>; ///< Тип словаря.

/**
 * @brief Получить входные данные с клавиатуры.
 *
 * @param[out] startWord - Слово, с которого начинаются преобразования.
 * @param[out] finishWord - Слово, к которому нужно преобразовать.
 * @param[out] Dictionary - Словарь слов по которым мы можем преобразовывать.
 */
void readUserInputs(std::string& startWord, std::string& finishWord, dictionary_t& Dictionary)
{
  startWord.clear();
  finishWord.clear();
  Dictionary.clear();

  std::cout << "Print your start and finish words:\n\ts: ";
  std::cin >> startWord;
  std::cout << "\tf: ";
  std::cin >> finishWord;

  std::cout << "Your start and finish words:\n";
  std::cout << "\ts: " << startWord << "\n\tf: " << finishWord << "\n";

  std::string input;
  std::cout << "Print your dictionary (delimeter == ','):\n\tD: ";
  std::cin >> input;

  while (input != "")
  {
    Dictionary.push_back(treeNode{.word = input.substr(0, input.find(',')), .isUsed = false, .depthDegree = 0});
    input.erase(0, input.find(','));
    input.erase(0, 1);
  }
}

/**
 * @brief Удалить слова, отличающейся длиной от стартового слова (т.к. по условию мы не можем в ходе преобразований
 * менять длину слова).
 *
 * @param[in,out] Dictionary - Словарь слов по которым мы можем преобразовывать.
 * @param[in] targetLenght - Все слова не равные данной длине будут удалены.
 */
void deleteWordsWithImposibleTransform(dictionary_t& Dictionary, const size_t targetLenght)
{
  for (size_t i = 0; i < Dictionary.size();)
  {
    if (Dictionary[i].word.length() != targetLenght)
    {
      Dictionary.erase(Dictionary.begin() + i);
      continue;
    }

    ++i;
  }
}

/**
 * @brief Удалить повторяющиеся слова из словаря.
 *
 * @param[in,out] Dictionary - Словарь слов по которым мы можем преобразовывать.
 */
void deleteRepeatableWords(dictionary_t& Dictionary)
{
  for (size_t i = 0; i < Dictionary.size(); i++)
    for (size_t j = i + 1; j < Dictionary.size();)
    {
      if (Dictionary[i].word == Dictionary[j].word)
      {
        Dictionary.erase(Dictionary.begin() + j);
        continue;
      }

      ++j;
    }
}

/**
 * @brief Проверить, что стартовый и финишный слова находятся в словаре.
 *
 * @param[in] startWord - Слово, с которого начинаются преобразования.
 * @param[in] finishWord - Слово, к которому нужно преобразовать.
 * @param[in] Dictionary - Словарь слов по которым мы можем преобразовывать.
 * @return Находятся ли оба слова с словаре или нет.
 */
bool isStartAndFinishWordContained(const std::string& startWord, const std::string& finishWord,
                                   const dictionary_t& Dictionary)
{
  bool isStartWordContained = false, isFinishWordContained = false;

  for (const treeNode& curNode : Dictionary)
  {
    if (curNode.word == startWord)
      isStartWordContained = true;

    if (curNode.word == finishWord)
      isFinishWordContained = true;

    if (isStartWordContained && isFinishWordContained)
      break;
  }

  return isStartWordContained && isFinishWordContained;
}

/**
 * @brief Проверить введенные пользователем данные на возможность преобразований. Произвести необходимые
 * подготовительные действия.
 *
 * @param[in] startWord - Слово, с которого начинаются преобразования.
 * @param[in] finishWord - Слово, к которому нужно преобразовать.
 * @param[in,out] Dictionary - Словарь слов по которым мы можем преобразовывать.
 * @return Введенные пользователем данные валидны или нет.
 */
bool validUserInputs(const std::string& startWord, const std::string& finishWord, dictionary_t& Dictionary)
{
  if (startWord.length() != finishWord.length())
  {
    std::cout << "Transform in not posible (s.length() != f.length())\n";
    return false;
  }

  if (!isStartAndFinishWordContained(startWord, finishWord, Dictionary))
  {
    std::cout << "Start or/and finish word is not contained in dictionary\n";
    return false;
  }

  deleteWordsWithImposibleTransform(Dictionary, startWord.length());

  deleteRepeatableWords(Dictionary);

  return true;
}

/**
 * @brief Проверить, что слова отличаются ровно на один символ.
 *
 * @param[in] a,b Слова, которые нужно проверить.
 * @return Слова отличаются ровно на один символ или нет.
 */
bool isDifferentByOne(const std::string& a, const std::string& b)
{
  uint8_t different = 0;

  for (size_t i = 0; i < a.length(); i++)
  {
    if (a[i] != b[i])
      ++different;

    if (different > 1)
      break;
  }

  return different == 1;
}

/**
 * @brief Рассчитать кратчайщие длины путей трансформации для каждого слова.
 *
 * @param[in] startWord - Слово, с которого начинаются преобразования.
 * @param[in,out] Dictionary - Словарь слов по которым мы можем преобразовывать.
 */
void calculateLengthTransformPath(const std::string& startWord, dictionary_t& Dictionary)
{
  for (treeNode& curNode : Dictionary)
  {
    if (curNode.word != startWord)
      continue;

    curNode.isUsed = true;
    curNode.depthDegree = 1;
    break;
  }

  size_t curDepthDegree = 1;
  bool anotherTranformIsPosible = false;

  do
  {
    anotherTranformIsPosible = false;

    for (treeNode& curNode : Dictionary)
    {
      if (curNode.depthDegree != curDepthDegree)
        continue;

      for (treeNode& anotherNode : Dictionary)
      {
        if (anotherNode.isUsed)
          continue;

        if (!isDifferentByOne(curNode.word, anotherNode.word))
          continue;

        anotherNode.isUsed = true;
        anotherNode.depthDegree = curDepthDegree + 1;

        anotherTranformIsPosible = true;
      }
    }

    ++curDepthDegree;
  } while (anotherTranformIsPosible);

  return;
}

/**
 * @brief Получить длину кротчайшего расстояния трансформации, если такой существует.
 *
 * @param[in] Dictionary - Словарь слов по которым мы можем преобразовывать.
 * @param[in] startWord - Слово, с которого начинаются преобразования.
 * @param[in] finishWord - Слово, к которому нужно преобразовать.
 * @return Строка с ответом на вопрос.
 */
std::string getShortedLengthTransformPath(const dictionary_t& Dictionary, const std::string& startWord,
                                          const std::string& finishWord)
{
  std::string answer{};

  auto answerIterator = std::find_if(Dictionary.begin(), Dictionary.end(),
                                     [&finishWord](const treeNode& a) { return a.word == finishWord; });

  if (!answerIterator->isUsed)
  {
    answer = "Transform in not posible";
  }
  else
  {
    answer = "Shorter length path to transform \'" + startWord + "\' to \'" + finishWord +
             "\': " + std::to_string(answerIterator->depthDegree - 1);
  }

  return answer;
}

/**
 * @brief Вывести словарь преобразований на консоль.
 *
 * @param[in] Dictionary - Словарь слов по которым мы можем преобразовывать.
 */
void printDictionaryToConsole(const dictionary_t& Dictionary)
{
  std::cout << "\nYour dictionary:\n\tD: ";

  for (const treeNode& curNode : Dictionary)
    std::cout << curNode.word << " ";

  std::cout << "\n";
}

int main()
{
  std::string startWord{}, finishWord{};
  dictionary_t Dictionary{};

  do
  {
    readUserInputs(startWord, finishWord, Dictionary);
  } while (!validUserInputs(startWord, finishWord, Dictionary));

  printDictionaryToConsole(Dictionary);

  calculateLengthTransformPath(startWord, Dictionary);

  std::cout << getShortedLengthTransformPath(Dictionary, startWord, finishWord) << "\n";

  return 0;
}