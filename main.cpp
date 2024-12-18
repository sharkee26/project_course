#include <gtk/gtk.h>
#include <string>

class ChangeColorsApp {
public:
    ChangeColorsApp() {
        app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(app, "activate", G_CALLBACK(activate), this);
    }

    int run(int argc, char **argv) {
        return g_application_run(G_APPLICATION(app), argc, argv);
    }

    ~ChangeColorsApp() {
        g_object_unref(app);
    }

private:
    GtkApplication *app;
    GtkWidget *window;

    static void on_color_changed(GObject *dropdown, GParamSpec *pspec, gpointer user_data) {
        auto *self = static_cast<ChangeColorsApp*>(user_data);

        // Получаем выбранный цвет
        GtkStringObject *selected_item = GTK_STRING_OBJECT(gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown)));
        const gchar *color_name = gtk_string_object_get_string(selected_item);

        // Меняем цвет фона окна
        self->set_background_color(color_name);
    }

    static void activate(GtkApplication *app, gpointer user_data) {
        auto *self = static_cast<ChangeColorsApp*>(user_data);
        self->create_window();
    }

    void create_window() {
        window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "Colors");
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

        const char *colors[] = {"White", "Red", "Blue", "Green", "Yellow", nullptr}; 
        GtkStringList *color_list = gtk_string_list_new(colors); 

        // Создаем выпадающий список
        GtkWidget *dropdown = gtk_drop_down_new(G_LIST_MODEL(color_list), nullptr); 

        // Подключаем сигнал изменения цвета
        g_signal_connect(dropdown, "notify::selected-item", G_CALLBACK(on_color_changed), this);

        // Устанавливаем выпадающий список как дочерний элемент окна
        gtk_window_set_child(GTK_WINDOW(window), dropdown);

        gtk_window_present(GTK_WINDOW(window));
    }

    void set_background_color(const char* color) {
        GtkCssProvider *provider = gtk_css_provider_new();
        
        // Формируем CSS-строку для изменения цвета фона
        std::string css = std::string("window { background-color: ") + color + "; }";
        
        // Загружаем CSS-правила
        gtk_css_provider_load_from_string(provider, css.c_str());
        
        // Применяем CSS-провайдер к контексту стиля окна
        GdkDisplay *display = gdk_display_get_default();
        
        // Добавляем провайдер для текущего дисплея
        gtk_style_context_add_provider_for_display(display,
                                                   GTK_STYLE_PROVIDER(provider),
                                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
                                                   
        g_object_unref(provider);
    }
};

int main(int argc, char **argv) {
    ChangeColorsApp app;
    return app.run(argc, argv);
}
