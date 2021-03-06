# MelomanButtons
## Проводной вариант
Сообщения отправляются в виде напечатанных на клавиатуре символов. Нажатие кнопок: '0' - '3'. Низкий уровень зараяда батарейки: "l0" - "l3" ('l' - прописная латинская эль).  
Главная кнопка имеет батарейку для питания радиоприёмника, то есть сама кнопка может работать и без неё, но в таком случае кнопка будет всё равно посылать сигналы о низком заряде батарейки. Поэтому рекомендуется сначала подключить батарейку и включить питание, а потом подключать через шнур.
## Блютуз вариант
### Подключение
Имя устройства: HC-05

Пароль: 1234
### Команды

Все данные передаются в текстовом виде, используя однобайтовую кодировку. Каждая команда состоит из двух символов без разделительного пробела и завершающего символа. Ответы на полученные команды не отправляются. 

Первый символ означает номер кнопки, с которой пришли данные или для которой предназначается команда: ‘0’ – ‘3’. Также в одном случае возможен символ ‘b’ означающий, что сообщение от блютуза.

Второй символ представляет собой прописную латинскую букву, которая описывает непосредственно событие от указанной кнопки или команду для неё.

Символ|Значение|Замечание
------|--------|---------
Входящие
‘p’|Произошло нажатие кнопк|Автоматически ограничивается частота нажатия кнопки, примерно раз в секунду
‘l’ (эль)|Низкий уровень заряда батарейки|Устройство будет продолжать работать, но возможны потери данных. Также может прийти от блютуза. Пример: “bl”
Исходящие
‘r’|Зажечь красный светодиод|Будет гореть пока не примет команду погасить. Если примет другую команду работы со светом – выполнит её, предварительно погаснув
‘g’|Зажечь зелёный светодиод|-- / / --
‘y’|Мигнуть красным светодиодом|Светодиод прогорит 0,2 секунды и погаснет
‘u’|Мигнуть зелёным светодиодом|-- / / --
‘n’|Погасить	
### Дополнения
У главной кнопки две батарейки, одна питает саму кнопку, вторая блютуз. Разряжаются они независимо. 

При ещё более низком уровне заряда у кнопки дополнительно к сообщению будет мигать красный светодиод, если низкий уровень у блютуза, то зелёный.
