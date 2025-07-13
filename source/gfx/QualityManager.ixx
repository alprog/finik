export module quality_manager;

import singleton;
export import quality_settings;

export class QualityManager : public Singleton<QualityManager>
{
public:
    const QualitySettings& getCurrent()
    {
        return settings;
    }

    void apply(QualitySettings settings);

protected:
    QualitySettings settings;
};