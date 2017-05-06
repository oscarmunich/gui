import QtQuick 2.5
import "../../js/Styler.js" as Styler

TableField {
    id: root

    color: Styler.Colors.TableHeader.Color
    fontSize: Styler.Fonts.TableHeader.PixelSize
    fontBold: Styler.Fonts.TableHeader.Weight === "Bold"
    fontColor: Styler.Fonts.TableField.Color
}
