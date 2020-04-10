{{ name | escape | underline }}

.. currentmodule:: {{ module }}

.. autoclass:: {{ objname }}

   {% block methods %}
   {% if methods %}
   .. rubric:: Methods

   .. autosummary::
      :nosignatures:
   {% for item in methods %}
      ~{{ name }}.{{ item }}
   {%- endfor %}
   {% endif %}
   {% endblock %}

   {% block attributes %}
   {% if attributes %}
   .. rubric:: Attributes

   .. autosummary::
   {% for item in attributes %}
      ~{{ name }}.{{ item }}
   {%- endfor %}
   {% endif %}
   {% endblock %}

{% block methods_detail %}
{% if methods %}

Methods
-------

{% for item in methods %}

.. automethod:: {{ module }}.{{ objname }}.{{ item }}

{%- endfor %}
{% endif %}
{% endblock %}

{% block attributes_detail %}
{% if attributes %}

Attributes
----------

{% for item in attributes %}

.. autoattribute:: {{ module }}.{{ objname }}.{{ item }}

{%- endfor %}
{% endif %}
{% endblock %}
