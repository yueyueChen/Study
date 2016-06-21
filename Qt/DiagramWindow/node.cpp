#include "node.h"
#include <QTWidgets>

Node::Node()
{
    myTextColor       = Qt::darkGreen;
    myOutlineColor    = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsSelectable|ItemIsMovable);
}
Node::~Node()
{
    foreach (Link *link,myLinks)
        delete link;
}
void Node::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}
void Node::SetTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}
void Node::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
}
void Node::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
}
void Node::addLink(Link *link)
{
    myLinks.insert(link);
}
void Node::removeLink(Link *link)
{
    myLinks.remove(link);
}
QRectF Node::outLineRect()const
{
    const int padding = 8;
    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect(myText);
    rect.adjust(-padding,-padding,+padding,+padding);
    rect.translate(-rect.center());
    return rect;
}
QRectF Node::boundingRect()const
{
    const int Margin = 1;
    return outLineRect().adjusted(-Margin,-Margin,+Margin,+Margin);
}
QPainterPath Node::shape()const
{
    QRectF rect = outLineRect();
    QPainterPath path;
    path.addRoundRect(rect,roundness(rect.width()),roundness(rect.height()));
    return path;
}
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(myOutlineColor);
    if(option->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);
    QRectF rect = outLineRect();
    painter->drawRoundedRect(rect,roundness(rect.width()),roundness(rect.height()));

    painter->setPen(myTextColor);
    painter->drawText(rect,Qt::AlignCenter,myText);
}
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged)
    {
        foreach (Link *link,myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change,value);
}
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),tr("Edit Text"),tr("Enter new text:"),
                                         QLineEdit::Normal,myText);
    if(!text.isEmpty())
        setText(text);
}
int Node::roundness(double size) const
{
    const int Diameter = 12;
    return 100 * Diameter /int(size);
}
