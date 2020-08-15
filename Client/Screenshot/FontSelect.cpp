#include "FontSelect.h"
#include <QColorDialog>

FontSelect::FontSelect(QWidget *parent)
    : QWidget(parent),
    m_isBoldClick{false},
    m_isItalicClick{false},
    m_isUnderlineClick{false}
{
    this->resize(500, 40);
    this->setStyleSheet("font-family:Microsoft YaHei;background-color:#16181C;");
    this->setWindowFlags(Qt::FramelessWindowHint);
    fontComboBox = new QFontComboBox;
    fontComboBox->setFixedHeight(32);
    fontComboBox->setFontFilters(QFontComboBox::ProportionalFonts);
    fontComboBox->setStyleSheet("QComboBox{border:1px solid #E6E6E6 ; color:white;padding-right: 0px;font-family:Microsoft YaHei}\
        QComboBox:hover{ border:1px solid 	#599AFE;  }\
        QComboBox:pressed{border:1px solid 	#599AFE ;}\
        QComboBox:drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 30px;border-left-width: 0px;}\
        QComboBox:down-arrow{width:16px;height:16px;image:url(:/Resources/clickxiala.png);}\
        QComboBox:down-arrow:pressed{image: url(:/Resources/xiala.png);}\
        QComboBox QAbstractItemView{outline:0px;font-size:12px;border:1px solid rgba(0,0,0,10%);border-radius: 5px;padding:5px;background-color: #FFFFFF;}\
        QAbstractItemView::item{height: 36px;}\
        QAbstractItemView::item:selected{background:rgba(0,0,0,3%);color:#000000;}");
    fontComboBox->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    fontComboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    sizeComboBox = new QComboBox;
    sizeComboBox->setFixedHeight(32);
    sizeComboBox->setStyleSheet("QComboBox{border:1px solid #E6E6E6 ;border-radius: 5px; color:white;padding-right: 0px;font-family:Microsoft YaHei;font-size:12px}\
        QComboBox:hover{ border:1px solid 	#599AFE; border-radius: 5px; }\
        QComboBox:pressed{border:1px solid 	#599AFE ;border-radius: 5px;}\
        QComboBox:drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 30px;border-radius: 3px;border-left-width: 0px;}\
        QComboBox:down-arrow{width:16px;height:16px;image:url(:/Resources/clickxiala.png);}\
        QComboBox:down-arrow:pressed{image: url(:/Resources/xiala.png);}\
        QComboBox QAbstractItemView{outline:0px;font-size:12px;border:1px solid rgba(0,0,0,10%);border-radius: 5px;padding:5px;background-color: #FFFFFF;}\
        QAbstractItemView::item{height: 36px;}\
        QAbstractItemView::item:selected{background:rgba(0,0,0,3%);color:#000000;}");
    sizeComboBox->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    sizeComboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    QFontDatabase db;
    foreach(int size, db.standardSizes()) {
        sizeComboBox->addItem(QString::number(size));
    }
    sizeComboBox->setCurrentIndex(6);
    m_hLayout = new QHBoxLayout(this);

    boldBtn = new QToolButton;
    boldBtn->setCursor(QCursor(Qt::PointingHandCursor));
    boldBtn->setAutoRaise(true);
    boldBtn->setFixedSize(32, 32);
    boldBtn->setIcon(QIcon(":/Resources/bold.png"));
    //boldBtn->setCheckable(true);

    italicBtn = new QToolButton;
    italicBtn->setCursor(QCursor(Qt::PointingHandCursor));
    italicBtn->setAutoRaise(true);
    italicBtn->setFixedSize(32, 32);
    italicBtn->setIcon(QIcon(":/Resources/italicBtn.png"));
    //italicBtn->setCheckable(true);
    underlineBtn = new QToolButton;
    underlineBtn->setCursor(QCursor(Qt::PointingHandCursor));
    underlineBtn->setAutoRaise(true);
    underlineBtn->setFixedSize(32, 32);
    underlineBtn->setIcon(QIcon(":/Resources/underlineBtn.png"));
    //underlineBtn->setCheckable(true);
    colorBtn = new QToolButton;
    colorBtn->setCursor(QCursor(Qt::PointingHandCursor));
    colorBtn->setAutoRaise(true);
    colorBtn->setFixedSize(32, 32);
    colorBtn->setIcon(QIcon(":/Resources/colorBtn.png"));
    //colorBtn->setCheckable(true);
    m_hLayout->setContentsMargins(10, 0, 10, 0);
    m_hLayout->addWidget(fontComboBox);
    m_hLayout->addWidget(sizeComboBox);
    m_hLayout->addWidget(boldBtn);
    m_hLayout->addWidget(italicBtn);
    m_hLayout->addWidget(underlineBtn);
    //m_hLayout->addWidget(colorBtn);
    QObject::connect(colorBtn, &QToolButton::clicked, this, &FontSelect::ShowColorBtn);
    QObject::connect(sizeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotCurrentIndexChanged(const QString &)));
    QObject::connect(fontComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotFontIndexChanged(const QString &)));
    QObject::connect(boldBtn, &QToolButton::clicked, [=]() {
        if (!m_isBoldClick)
        {
            boldBtn->setStyleSheet("border:1px solid #1E90FF;");
            m_isBoldClick = !m_isBoldClick;
            emit emitbtnClicked(m_isBoldClick);
        }
        else
        {
            boldBtn->setStyleSheet("border:0px;");
            m_isBoldClick = !m_isBoldClick;
            emit emitbtnClicked(m_isBoldClick);
        }});
    QObject::connect(italicBtn, &QToolButton::clicked, [=]() {
        if (!m_isItalicClick)
        {
            italicBtn->setStyleSheet("border:1px solid #1E90FF;");
            m_isItalicClick = !m_isItalicClick;
            emit emitItalicClick(m_isItalicClick);
        }
        else
        {
            italicBtn->setStyleSheet("border:0px;");
            m_isItalicClick = !m_isItalicClick;
            emit emitItalicClick(m_isItalicClick);
        }});
    QObject::connect(underlineBtn, &QToolButton::clicked, [=]() {
        if (!m_isUnderlineClick)
        {
            underlineBtn->setStyleSheet("border:1px solid #1E90FF;");
            m_isUnderlineClick = !m_isUnderlineClick;
            emit emitUnderlineClick(m_isUnderlineClick);
        }
        else
        {
            underlineBtn->setStyleSheet("border:0px;");
            m_isUnderlineClick = !m_isUnderlineClick;
            emit emitUnderlineClick(m_isUnderlineClick);
        }});
}

void FontSelect::ShowColorBtn()
{
    QColorDialog colorD;
    //colorD.setGeometry(200, 0, 500, 500);
    QColor color = colorD.getColor(Qt::red);
    if (color.isValid())
    {
        emit emitColor(color);
        colorBtn->setStyleSheet("border:1px solid #1E90FF;");
    }
}

void FontSelect::slotCurrentIndexChanged(const QString & str)
{
    int size = str.toInt();
    emit emitSize(size);
}

void FontSelect::slotFontIndexChanged(const QString & str)
{
    emitFontFamily(str);
}

FontSelect::~FontSelect()
{
    delete fontComboBox;
    fontComboBox = nullptr;
    delete sizeComboBox;
    sizeComboBox = nullptr;
    delete boldBtn;
    boldBtn = nullptr;
    delete italicBtn;
    italicBtn = nullptr;
    delete underlineBtn;
    underlineBtn = nullptr;
    delete colorBtn;
    colorBtn = nullptr;
    delete m_hLayout;
    m_hLayout = nullptr;
}
