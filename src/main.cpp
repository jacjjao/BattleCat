#include "App.hpp"

#include "Core/Context.hpp"

#include "Entity.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;
    auto cat = Entity<CatType>::CreateCat(CatStats::Cat, CatType::CAT, {}, 1,
                                          [](const Entity<CatType> &) {});
    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;

            case App::State::UPDATE:
                app.Update();
                break;

            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
    }
    return 0;
}
