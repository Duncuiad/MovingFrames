#include "EditorWidget.h"

void EditorWidget::ConnectOnChangedCallback(const OnChangedCallback& anOnChanged)
{
    myOnChanged = anOnChanged;
}
