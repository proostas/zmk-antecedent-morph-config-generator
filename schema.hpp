#ifndef SCHEMA_HPP
#define SCHEMA_HPP

#include <QObject>
#include <QString>
#include <memory>
#include <QHash>
#include <QTextStream>

class Antecedent;
class Layer;
class Morph;

using LayerPtr = std::shared_ptr<Layer>;
using MorphPtr = std::shared_ptr<Morph>;
using AntecedentPtr = std::shared_ptr<Antecedent>;

class Schema : public QObject
{
    Q_OBJECT

public:
    enum Type {
        UnknownType,
        Flat,
        Deep
    };
    enum Kind {
        UnknownKind,
        English,
        Cpp,
        CMake
    };
    enum Direction {
        NorthEast,
        East,
        SouthEast,
        SouthWest,
        West,
        NorthWest
    };

public:
    explicit Schema(QObject *parent);

    bool isValid() const;
    QString name() const { return m_name; }
    QString version() const { return m_version; }

    void generateConfig(QTextStream& out) const;

    bool load(QString const& filePath);
    bool save(QString const& filePath);
    void clear();

    static QString behaviorNodeLabel(Kind kind);
    static QString behaviorNodeName(Kind kind);
    static QString behaviorLabel(Kind kind);
    static QString behavior(QString const& nodeLabel, QString const& nodeName, QString const& label, QString const& bindings, QString const& antecedents);

signals:
    void logEvent(QString const& message) const;

private:
    bool readCSVRow(QTextStream &in, QStringList *row);
    void generateBaseLayerBehaviors(QTextStream& out) const;
    void generateMouseLayerBehaviors(QTextStream& out) const;
    void generateNavigationLayerBehaviors(QTextStream& out) const;
    void generateMediaLayerBehaviors(QTextStream& out) const;
    void generateFunctionLayerBehaviors(QTextStream& out) const;
    void generateNumberLayerBehaviors(QTextStream& out) const;
    void generateSymbolLayerBehaviors(QTextStream& out) const;
    void generateMacros(QTextStream& out) const;

private:
    Type m_type{UnknownType};
    Kind m_kind{UnknownKind};
    QString m_name;
    QString m_version;
    QHash<QString,AntecedentPtr> m_antecedents;
    AntecedentPtr m_curAntecedent{};
    LayerPtr m_curLayer{};
    QString m_behaviorTemplate;
    QHash<QString,Kind> m_kindMap;
    std::vector<QString> m_orderedSymbols;
};

class Layer
{
public:
    enum Type {
        Unknown,
        Mouse,
        Navigation,
        Media,
        Function,
        Number,
        Symbol
    };

public:
    Layer() = default;
    Layer(QString label);

    Type type() const { return m_type; }

    static int mapDirectionToIndex(Schema::Direction dir, Type layerType);
    static bool isLayer(QString const& v);
    static QString behaviorNodeLabel(Schema::Direction dir, Type layerType = Unknown);
    static QString behaviorNodeName(Schema::Direction dir, Type layerType = Unknown);
    static QString behaviorLabel(Schema::Direction dir, Type layerType = Unknown);

    void addMorph(QString const& phrase, QHash<QString,bool>& labels);

    bool isEmpty(Schema::Direction dir) const;
    bool isEmpty() const;
    QStringList morphNodeLabels() const;
    QString binding(QString const& prefix, QString const& symbol, Schema::Direction dir) const;
    QStringList macros(QString const& symbol, QString const& prefix) const;

private:
    Type m_type{Unknown};
    QString m_label;
    std::vector<MorphPtr> m_morphs;
    static QHash<QString,Type> s_typeMap;
};

class Antecedent
{
public:
    Antecedent() = default;
    explicit Antecedent(QString symbol);

    LayerPtr addLayer(QString const& label);
    void addMorph(QString const& phrase, QHash<QString,bool>& labels);

    bool isValid(Schema::Type const schemaType) const;
    bool isEmpty(Schema::Direction dir, Layer::Type layerType = Layer::Unknown) const;
    bool isEmpty() const;
    QString symbol() const { return m_symbol; }
    QString keycode() const;

    QStringList morphNodeLabels() const;
    QString binding(QString const& prefix, Schema::Direction dir, Layer::Type layerType = Layer::Unknown);
    QStringList macros(QString const& prefix) const;

private:
    QString m_symbol;
    QHash<Layer::Type,LayerPtr> m_layers;
    std::vector<MorphPtr> m_morphs;
    static std::vector<Layer::Type> s_orderedLayers;
};

class Morph
{
public:
    explicit Morph(QString const& phrase);

    void incrementPostfix();

    bool isEmpty() const;
    bool isSingleLettered(QString const& symbol) const;
    QString nodeLabel() const;
    QString binding(QString const& symbol, QString const& prefix) const;
    QString macro(QString const& symbol, QString const& prefix) const;

    static QString zmkKeycode(QString const& c);

private:
    QString m_phrase;
    int m_postfix;
};

#endif // SCHEMA_HPP
