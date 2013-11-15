<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
<context>
    <name>Messages</name>
    <message>
        <source>__context_printpass_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Не хватает параметров&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Вместе с параметром &lt;i&gt;&amp;#8209;&amp;#8209;print&amp;#8209;pass&lt;/i&gt; необходимо использовать параметр &lt;i&gt;&amp;#8209;&amp;#8209;description&lt;/i&gt; или &lt;i&gt;&amp;#8209;&amp;#8209;message&lt;/i&gt;</translation>
    </message>
    <message>
        <source>__modevalidator_nomodes_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Нужные методы отсутствуют&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Не было найдено ни однога метода смены пользователя</translation>
    </message>
    <message>
        <source>__modevalidator_wrongmode_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Несуществующий метод&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Выбранный метод смены пользователя не найден</translation>
    </message>
    <message>
        <source>__pswchecker_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Не удалось проверить пароль&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Сведения о пароле не могут быть получены</translation>
    </message>
    <message>
        <source>__pswchecker_not_sudoer_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Нет записи в sudoers&lt;/b&gt;&lt;/big&gt;&lt;br&gt;У текущего пользователя отсутствует необходимая для sudo авторизация</translation>
    </message>
    <message>
        <source>__context_verbosity_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Неправильная детализация&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Уровень детализации должен быть числом от 0 до 4</translation>
    </message>
    <message>
        <source>__context_wordexp_wrn__</source>
        <translation>Внимание: Не удалось интерпретировать команду (оставлена как есть)</translation>
    </message>
    <message>
        <source>__context_descfile_wrn%1%2__</source>
        <translation>Внимание: Невозможно загрузить ключ &quot;%1&quot; из desktop файла: %2</translation>
    </message>
    <message>
        <source>__runtools_fork_err_BASE%1%2__</source>
        <translation>Ошибка во время запуска %1: %2</translation>
    </message>
    <message>
        <source>__runtools_fork_err_BADPSW%1__</source>
        <translation>Ошибка: %1 требует ввести пароль</translation>
    </message>
    <message>
        <source>__runtools_fork_wrn_NOPSW%1__</source>
        <translation>Внимание - для %1 пароль не потребовался</translation>
    </message>
    <message>
        <source>__runtools_fork_wrn_ECODE%1%2%3__</source>
        <translation>Внимание - %1: Получен exit code %2: %3</translation>
    </message>
    <message>
        <source>__uservalidator_wronguser_err%1__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Несуществующий пользователь&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Пользователь %1 не найден</translation>
    </message>
    <message>
        <source>__context_badinvoke_wrn__</source>
        <translation>Внимание: Не удалось интерпретировать параметры invoker (применятся в команде без изменений)</translation>
    </message>
    <message>
        <source>__context_nodesktop_err__</source>
        <translation>&lt;big&gt;&lt;b&gt;Ошибка: Неверный аргумент&lt;/b&gt;&lt;/big&gt;&lt;br&gt;Вместе с параметром &lt;i&gt;&amp;#8209;&amp;#8209;force&amp;#8209;desktop&lt;/i&gt; необходимо использовать существующее имя файла в параметрах &lt;i&gt;&amp;#8209;&amp;#8209;description&lt;/i&gt; или &lt;i&gt;&amp;#8209;&amp;#8209;message&lt;/i&gt;</translation>
    </message>
</context>
<context>
    <name>PassPage</name>
    <message>
        <source>__enter_pass__</source>
        <translation>Введите пароль</translation>
    </message>
    <message>
        <source>__launch__</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>__pass_ok__</source>
        <translation>Пароль подтвержден. Программа запускается...</translation>
    </message>
    <message>
        <source>__pass_wrong_end__</source>
        <translation>Неверный пароль. Все попытки израсходованы.</translation>
    </message>
    <message>
        <source>__pass_not_needed__</source>
        <translation>Пароль не требуется</translation>
    </message>
    <message>
        <source>__default_sudo_root_msg%D__</source>
        <translation>Введите ваш пароль чтобы разрешить приложению «%D» изменять ключевые параметры вашей системы</translation>
    </message>
    <message>
        <source>__default_root_msg%D__</source>
        <translation>Введите административный пароль чтобы разрешить приложению «%D» изменять ключевые параметры вашей системы</translation>
    </message>
    <message>
        <source>__default_sudo_msg%U%D__</source>
        <translation>Введите ваш пароль для запуска приложения «%D» с правами пользователя %U</translation>
    </message>
    <message>
        <source>__default_msg%U%D__</source>
        <translation>Введите пароль пользователя %U для запуска приложения «%D»</translation>
    </message>
    <message>
        <source>__pass_wrong_cnt%R__</source>
        <translation>Неверный пароль... введите снова. Попыток осталось: %R</translation>
    </message>
    <message>
        <source>__default_ariadne_msg%U%D__</source>
        <translation>Введите административный пароль для запуска приложения «%D» с правами пользователя %U</translation>
    </message>
</context>
<context>
    <name>StartPage</name>
    <message>
        <source>__start__</source>
        <translation>Запустить программу</translation>
    </message>
    <message>
        <source>__run__</source>
        <translation>Команда для запуска</translation>
    </message>
    <message>
        <source>__user__</source>
        <translation>Как пользователь</translation>
    </message>
    <message>
        <source>__login_switch__</source>
        <translation>Оболочка регистрации</translation>
    </message>
    <message>
        <source>__env_switch__</source>
        <translation>Сохранить окружение</translation>
    </message>
    <message>
        <source>__mode_select__</source>
        <translation>С помощью</translation>
    </message>
    <message>
        <source>__advanced__</source>
        <translation>Дополнительно</translation>
    </message>
    <message>
        <source>__common__</source>
        <translation>Общие</translation>
    </message>
    <message>
        <source>__modes_list__</source>
        <translation>Методы</translation>
    </message>
    <message>
        <source>__users_list__</source>
        <translation>Пользователи</translation>
    </message>
    <message>
        <source>__command_text_empty__</source>
        <translation>Внимание: Команда не была задана</translation>
    </message>
    <message>
        <source>__done__</source>
        <translation>Готово</translation>
    </message>
    <message>
        <source>__open_app__</source>
        <translation>Выбрать</translation>
    </message>
    <message>
        <source>__cancel_app__</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <source>__select_app__</source>
        <translation>Выберите приложение</translation>
    </message>
</context>
<context>
    <name>main</name>
    <message>
        <source>__error_close__</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <source>__hmtsu_description__</source>
        <translation>&lt;style type=&apos;text/css&apos;&gt;a:link{color:#5183d7}&lt;/style&gt;HMTsu это аналог gksu, графическая оболочка для запуска devel&amp;#8209;su, sudo и ariadne. Для получения дополнительной информации &lt;a href=&quot;https://github.com/lcferrum/hmtsu&quot;&gt;посетите страницу проекта на GitHub&apos;е&lt;/a&gt;.</translation>
    </message>
</context>
</TS>
